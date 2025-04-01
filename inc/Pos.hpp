#pragma once

#include <iostream>

struct Pos
{
	uint	x;
	uint	y;

	Pos() {}
	Pos(const uint x_in, const uint y_in): x(x_in), y(y_in) {}

	bool	operator==(const Pos &pos);
	bool	operator!=(const Pos &pos);
};

std::ostream	&operator<<(std::ostream& o, const Pos& pos);
