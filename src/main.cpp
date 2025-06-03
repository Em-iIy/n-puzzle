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

void	print_results(Options &options, Square &base, std::vector<Result> &results)
{
	if (results.size() == 0)
		return ;
	std::cout << "\n\nFastest Solution" << std::endl;
	Result fastest = *(std::min_element(results.begin(), results.end(), [](Result a, Result b)
		{
			return (a.time_elapsed < b.time_elapsed);
		}
	));
	std::cout << fastest;

	std::cout << "\n\nBest Solution" << std::endl;
	Result best = *(std::min_element(results.begin(), results.end(), [](Result a, Result b)
		{
			if (a.solution.size() == b.solution.size())
				return (a.time_elapsed < b.time_elapsed);
			return (a.solution.size() < b.solution.size());
		}
	));
	std::cout << best;

	if (options._visualize)
	{
		Square sol = base;
		sol.print_board();
		std::cout << "------------------------------\n";
		for (e_move move: best.solution)
		{
			sol.make_move(move);
			sol.print_board();
			std::cout << "------------------------------\n";
		}
	}
}

int main(const int argc, char *argv[])
{
	erand::seed();
	try
	{
		Options options(argc, argv);

		Square base = options.build_square();
		std::vector<Result>	results;
		base.print_board();
		for (Node::Heuristic heuristic : options._heuristics)
		{
			for (Node::Type type : options._types)
			{
				try
				{
					Square sq(base);
					timer::start();
					Result result = AStar::solve(sq, heuristic, type);
					result.time_elapsed = timer::ms_elapsed();
					std::cout << result << std::endl;
					results.push_back(result);
				}
				catch(const std::bad_alloc &e)
				{
					std::cerr << "Memory limit " << options._mem_limit << "MB reached after " << timer::ms_elapsed() << "ms" << std::endl;
				}
			}
		}
		print_results(options, base, results);
	}
	catch(const std::exception& e)
	{
		std::cerr << "ERROR: " << e.what() << '\n';
		return (1);
	}
}
