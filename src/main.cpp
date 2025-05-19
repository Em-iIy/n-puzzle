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
#include "AStar.hpp"

void	usage(const std::string &arg0)
{
	std::cerr << "Usage:\t" << arg0 << " -f <filename>\n";
	std::cerr << "\t" << arg0 << " -n <num>\n";
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
		Square	goal;
		std::string opt = argv[1];
		if (opt == "-f")
		{
			std::vector<uint> grid = parse_puzzle(argv[2]);
			sq = Square(grid, init_solved_pos(sqrt(grid.size())));
			goal = Square(sq);
			goal.set_solved();
		}
		else if (opt == "-n")
		{
			int	n = std::stoi(argv[2]);
			if (n < 1)
				throw std::runtime_error("n must be greater than 0");
			shared_pos_vec final_positions = init_solved_pos(n);
			sq = Square(n, final_positions);
			goal = Square(sq);
			goal.set_solved();
			while (!sq.check_solvable(goal))
				sq = Square(n, final_positions);
		}
		else
		{
			usage(argv[0]);
			throw std::runtime_error("invalid usage\nOption " + opt + " does not exist.");
		}
		sq.print_board();
		if (!sq.check_solvable(goal))
			throw std::runtime_error("Puzzle is not solvable!");
		std::cout << "Solving puzzle...\n";


		timer::start();
		auto solution = AStar::solve(sq, Node::MANHATTAN, Node::ASTAR);
		double time_elapsed = timer::ms_elapsed();
		std::cout << "solution found in " << time_elapsed << " ms\n";
		std::cout << "moves required: " << solution.size() << std::endl;

		Square sol = sq;
		for (e_move move: solution)
		{
			sol.make_move(move);
			sol.print_board();
			std::cout << "------------------------------\n";
		}
	}
	catch(const std::exception& e)
	{
		std::cerr<< "ERROR: " << e.what() << '\n';
		return (1);
	}
}
