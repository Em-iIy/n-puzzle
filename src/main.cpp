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
#include "Options.hpp"

int main(const int argc, char *argv[])
{
	erand::seed();
	try
	{
		Options options(argc, argv);

		Square base = options.build_square();
		base.print_board();
		for (Node::Heuristic heuristic : options._heuristics)
		{
			for (Node::Type type : options._types)
			{
				Square sq(base);
				timer::start();
				auto solution = AStar::solve(sq, heuristic, type);
				double time_elapsed = timer::ms_elapsed();
				std::cout << "solution found in " << time_elapsed << " ms\n";
				std::cout << "moves required: " << solution.size() << std::endl;
			}
		}
		// Square sol = sq;
		// for (e_move move: solution)
		// {
		// 	sol.make_move(move);
		// 	sol.print_board();
		// 	std::cout << "------------------------------\n";
		// }
	}
	catch(const std::exception& e)
	{
		std::cerr << "ERROR: " << e.what() << '\n';
		return (1);
	}
	
}
