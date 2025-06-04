#include "Node.hpp"	

Node::Node() {}

Node::Node(const Node &src): _f(src._f), _g(src._g), _h(src._h), _sq(src._sq), _parent(src._parent)
{
}

void	Node::set_h(Heuristic heuristic)
{
	switch (heuristic)
	{
	case HAMMING:
		_h = _sq.hamming_distance();
		break;

	case MANHATTAN:
		_h = _sq.manhattan_distance();
		break;

	case LINEAR:
		_h = _sq.linear_conflict();
		break;

	default:
		break;
	}
}

void	Node::set_g()
{
	if (_parent)
		_g = _parent->_g + 1;
}

Node::Node(std::shared_ptr<Node> parent, Square &sq, Heuristic heuristic, Type type): _g(0), _h(0), _sq(sq), _parent(parent)
{
	switch (type)
	{
	case ASTAR:
		set_g();
		set_h(heuristic);
		break;

	case GREEDY:
		set_h(heuristic);
		break;

	case UNIFORM:
		set_g();
		break;
	
	default:
		break;
	}
	_f = _g + _h;
}

Node::~Node()
{
}
