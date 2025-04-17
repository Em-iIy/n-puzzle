#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include <thread>
#include <chrono>

#include "Square.hpp"
#include "Node.hpp"
#include "rand.hpp"
#include "timer.hpp"
#include "Parser.hpp"

void	usage(const std::string &arg0)
{
	std::cerr << "Usage:\t" << arg0 << " -f <filename>\n";
	std::cerr << "\t" << arg0 << " -n <num>\n";
}

std::shared_ptr<Node>	create_neighbor(std::shared_ptr<Node> parent, e_move move)
{
	Square	sq(parent->_sq);

	if (!sq.make_move(move))
		return (nullptr);
	return (std::make_shared<Node>(parent, sq));
}

std::shared_ptr<Node>	find_node_position(
	std::vector<std::shared_ptr<Node>> &set,
	std::shared_ptr<Node> current
)
{
	for (std::shared_ptr<Node> node : set)
	{
		if (node->_sq == current->_sq)
			return (node);
	}
	return (nullptr);
}

std::shared_ptr<Node>	find_best_f(std::vector<std::shared_ptr<Node>> &set)
{
	std::shared_ptr<Node>	best = nullptr;
	
	for (std::shared_ptr<Node> node : set)
		if (best == nullptr || node->_f < best->_f)
			best = node;
	return (best);
}


void	generate_neighbors(
	std::vector<std::shared_ptr<Node>> &open_set, 
	std::vector<std::shared_ptr<Node>> &closed_set,
	std::shared_ptr<Node> parent
)
{
	for (int i = 0; i < 4; ++i) // for each of the 4 moves
	{
		std::shared_ptr<Node>	neighbor = create_neighbor(parent, (e_move)i);
		if (neighbor == nullptr)
			continue ;
		std::shared_ptr<Node> closed_node = find_node_position(closed_set, neighbor);
		if (closed_node != nullptr)
			continue ;
		std::shared_ptr<Node> open_node = find_node_position(open_set, neighbor);
		if (open_node != nullptr)
		{
			if (open_node->_g > neighbor->_g)
			{
				open_node->_f = neighbor->_f;
				open_node->_g = neighbor->_g;
				open_node->_h = neighbor->_h;
				open_node->_parent = neighbor->_parent;
			}
		}
		else
		{
			open_set.push_back(neighbor);
		}
	}
}

void	a_star(Square &begin)
{
	std::vector<std::shared_ptr<Node>>	open_set;
	std::vector<std::shared_ptr<Node>>	closed_set;

	std::shared_ptr<Node> start = std::make_shared<Node>(nullptr, begin);
	open_set.push_back(start);
	while (open_set.size())
	{
		std::shared_ptr<Node> current = find_best_f(open_set);
		if (current->_sq.check_board())
		{
			current->_sq.print_board();
			std::cout << "SOLVED!!!!!!!!!" << std::endl;
			int count = 0;
			while (current != nullptr)
			{
				current = current->_parent;
				count++;
			}
			std::cout << "moves: " << count << std::endl;
			std::cout << "open " << open_set.size() << std::endl;
			std::cout << "closed " << closed_set.size() << std::endl;
			exit(0);
		}
		std::erase(open_set, current);
		closed_set.push_back(current);
		generate_neighbors(open_set, closed_set, current);
	}
}

int main(const int argc, char *argv[])
{
	erand::seed();
	if (argc != 3)
	{
		usage(argv[0]);
		return (1);
	}
	try
	{
		Square	sq;
		std::string opt = argv[1];
		if (opt == "-f")
		{
			std::vector<uint> grid = parse_puzzle(argv[2]);
			sq = Square(grid, init_solved_pos(sqrt(grid.size())));
		}
		else if (opt == "-n")
		{
			int	n = std::stoi(argv[2]);
			if (n < 1)
				throw std::runtime_error("n must be greater than 0");
			shared_pos_vec final_positions = init_solved_pos(n);
			sq = Square(n, final_positions);
			while (!sq.check_solvable())
				sq = Square(n, final_positions);
		}
		else
		{
			usage(argv[0]);
			throw std::runtime_error("invalid usage\nOption " + opt + " does not exist.");
		}
		sq.print_board();
		if (!sq.check_solvable())
			throw std::runtime_error("Puzzle is not solvable!");
		a_star(sq);
	}
	catch(const std::exception& e)
	{
		std::cerr<< "ERROR: " << e.what() << '\n';
		return (1);
	}
}
