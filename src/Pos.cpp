#include "Pos.hpp"

std::ostream	&operator<<(std::ostream& o, const Pos& pos)
{
	o << pos.x << "," << pos.y;
	return (o);
}

bool	Pos::operator==(const Pos &pos)
{
	return (this->x == pos.x && this->y == pos.y);
}

bool	Pos::operator!=(const Pos &pos)
{
	return (this->x != pos.x && this->y != pos.y);
}
