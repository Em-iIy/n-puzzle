#pragma once

#include <iostream>

struct Pos
{
	uint	x;
	uint	y;

	Pos() {}
	Pos(const uint x_in, const uint y_in): x(x_in), y(y_in) {}

	bool	operator==(const Pos &pos) const;
	bool	operator!=(const Pos &pos) const;
};

std::ostream	&operator<<(std::ostream& o, const Pos& pos);
