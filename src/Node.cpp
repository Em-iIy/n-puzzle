#include "Node.hpp"	

Node::Node() {}

Node::Node(const Node &src): _f(src._f), _g(src._g), _h(src._h), _sq(src._sq), _parent(src._parent)
{
}

Node::Node(std::shared_ptr<Node> parent, Square &sq): _sq(sq), _g(0), _h(0), _parent(parent)
{
	if (parent)
		_g = parent->_g + 1;
	_h = _sq.hamming_distance();
	_f = _g + _h;
}

Node::~Node()
{
}
