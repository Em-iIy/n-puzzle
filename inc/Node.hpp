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

		Node();
		Node(const Node &src);
		Node(std::shared_ptr<Node> parent, Square &sq);
		~Node();

		
};