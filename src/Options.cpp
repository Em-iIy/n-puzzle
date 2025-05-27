#include "Options.hpp"
#include "Parser.hpp"
#include "rand.hpp"

#include <cmath>

static void	usage(const std::string &arg0)
{
	std::cerr << "Usage:\t" << arg0 << " -f <filename>\n";
	std::cerr << "\t" << arg0 << " -n <num>\n";
	// options:
	// -n size			specify the lenght of one side of the board
	// -h heuristic		specify one heuristic to solve with (default = manhattan distance)
	//						manhattan
	//						hamming
	//						linear
	// -t type			specify alternative type
	//						default
	//						greedy
	//						uniform
	// -s number		specify how many moves you want to shuffle the board (-n must be given)
	// -f file			specify input file containing a puzzle
	// -v				visualize
}

Node::Heuristic	check_heuristic(const std::string &str)
{
	if (str == "manhattan")
		return (Node::MANHATTAN);
	if (str == "hamming")
		return (Node::HAMMING);
	if (str == "linear")
		return (Node::LINEAR);
	throw std::runtime_error("invalid heuristic");
}

Node::Type	check_type(const std::string &str)
{
	if (str == "default")
		return (Node::ASTAR);
	if (str == "greedy")
		return (Node::GREEDY);
	if (str == "uniform")
		return (Node::UNIFORM);
	throw std::runtime_error("invalid type");
}

Options::Options(int argc, char **argv)
{
	if (argc < 3)
	{
		usage(argv[0]);
		throw std::runtime_error("");
	}
	int opt;
	while ((opt = getopt(argc, argv, "n:h:t:s:f:v")) != -1)
	{
		switch (opt)
		{
			case 'n':
				_n = std::stoi(optarg);
				if (_n < 1)
					throw std::runtime_error("n must be greater than 0");
				break;
			case 'h':
				_heuristics.push_back(check_heuristic(std::string(optarg)));
				break;
			case 't':
				_types.push_back(check_type(std::string(optarg)));
				break;
			case 's':
				_shuffle = std::stoi(optarg);
				if (_shuffle < 1)
					throw std::runtime_error("shuffle must be greater than 0");
				break;
			case 'f':
				_file = true;
				_filename = optarg;
				break;
			case 'v':
				_visualize = true;
				break;
			default:
				throw std::runtime_error("invalid usage");
				break;
		}
	}
	// If no type or heuristic specified, set to defaults
	if (_heuristics.size() == 0)
		_heuristics.push_back(Node::MANHATTAN);
	if (_types.size() == 0)
		_types.push_back(Node::ASTAR);
}

Options::~Options() {};

Square	Options::build_square()
{
	Square sq;
	Square goal;
	if (_file)
	{
		std::vector<uint> grid = parse_puzzle(_filename);
		sq = Square(grid, init_solved_pos(sqrt(grid.size())));
		goal = Square(sq);
		goal.set_solved();
	}
	else
	{
		shared_pos_vec final_positions = init_solved_pos(_n);
		sq = Square(_n, final_positions);
		goal = Square(sq);
		goal.set_solved();
		while (!sq.check_solvable(goal))
			sq = Square(_n, final_positions);		
		if (_shuffle > 0)
		{
			sq.set_solved();
			std::uniform_int_distribution<int> gen = erand::generator(0, 3);
			e_move curr_move;
			e_move prev_move = (e_move)-1;
			for (int i = 0; i < _shuffle; ++i)
			{
				curr_move = (e_move)erand::rand(gen);
				if (curr_move == prev_move)
				{
					--i;
					continue ;
				}
				if (!sq.make_move(curr_move))
				{
					--i;
					continue ;
				}
				prev_move = curr_move;
			}
		}
	}
	if (!sq.check_solvable(goal))
		throw std::runtime_error("Puzzle is not solvable!");
	return (sq);
}