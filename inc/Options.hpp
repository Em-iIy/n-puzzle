#pragma once

#include "Node.hpp"
#include "Square.hpp"
#include <vector>


class Options
{
	public:
		std::vector<Node::Heuristic>	_heuristics;
		std::vector<Node::Type>		_types;
		bool						_file = false;
		std::string					_filename;
		int							_n;
		int32_t						_mem_limit = 0;
		int							_shuffle = 0;
		bool						_visualize = false;

		Options(int argc, char **argv);
		~Options();

		Square	build_square();
};