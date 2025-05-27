#pragma once

#include "Square.hpp"

class Node
{

	public:
		int						_f;
		int						_g;
		int						_h;
		Square					_sq;
		std::shared_ptr<Node>	_parent;

		enum Heuristic {
			HAMMING,
			MANHATTAN,
			LINEAR,
			DEFAULT
		};
		enum Type {
			ASTAR,
			GREEDY,
			UNIFORM
		};

		Node();
		Node(const Node &src);
		Node(std::shared_ptr<Node> parent, Square &sq, Heuristic heuristic, Type type);
		~Node();

	private:
		void	set_h(Heuristic heuristic);
		void	set_g();
};

inline bool operator>(const Node &a, const Node &b)
{
	return a._f > b._f;
}