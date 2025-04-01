#include <iostream>
#include <algorithm>
#include <cmath>
#include <cassert>

#include "Square.hpp"
#include "timer.hpp"
#include "rand.hpp"
#include "colour.h"

Square::Square() {}

// Initialize the square with the lenght of the side
Square::Square(uint n): _n((n * n) - 1), _len_side(n)
{
	_board.reserve(_n + 1);
	for (uint i = 0; i < _n + 1; ++i)
	{
		_board.push_back(i);
	}
	init_solved_pos();
	shuffle_board();
	_0 = get_pos(0);
}

// Initialize the square using a vector of unsigned ints
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Use this constructor after parsing in the file, I haven't tested if this works
Square::Square(std::vector<uint> &board): _n(board.size() - 1), _len_side(sqrt(board.size() - 1)), _board(board)
{
	init_solved_pos();
	// Find position of the 0 piece
	_0 = get_pos(0);
	// std::cout << "_n:\t" << _n << std::endl;
	// std::cout << "_len_side:\t" << _len_side << std::endl;
}


Square::~Square() {}

// Swaps the 0 piece up up, down, left or right
// Returns true if move was valid
bool		Square::make_move(e_move move)
{
	Pos	temp_pos = _0;
	switch (move)
	{
		case MOVE_UP:
			temp_pos.y--;
			break;
		case MOVE_DOWN:
			temp_pos.y++;
			break;
		case MOVE_LEFT:
			temp_pos.x--;
			break;
		case MOVE_RIGHT:
			temp_pos.x++;
			break;
		default:
			break;
	}
	if (!check_pos(temp_pos))
		return (false);
	std::swap(get_num(temp_pos), get_num(_0));
	_0 = temp_pos;
	return (true);
}

// Randomly shuffle the board's numbers
void		Square::shuffle_board()
{
	if (_len_side < 2)
		return ;
	std::uniform_real_distribution<double>	gen = erand::generator(0.0, _n + 1.0 - std::numeric_limits<float>::epsilon());
	for (uint i = 0; i < _n + 1; ++i)
	{
		std::swap(_board[i], _board[(uint)erand::rand(gen)]);
	}
}

// Initializes the positions for a solved board
void		Square::init_solved_pos()
{
	int left = 0;
	int right = _len_side - 1;
	int top = 0;
	int bottom = _len_side - 1;
	int	i = 0;
	
	_final_positions = std::vector<Pos>(_n + 1);
	while (top <= bottom && left <= right)
	{
		for (int x = left; x <= right; ++x)
			_final_positions[++i % (_n + 1)] = Pos(x, top);
		top++;
		for (int y = top; y <= bottom; ++y)
			_final_positions[++i % (_n + 1)] = Pos(right, y);
		right--;
		if (top <= bottom)
		{
			for (int x = right; x >= left; --x)
				_final_positions[++i % (_n + 1)] = Pos(x, bottom);
			bottom--;
		}
		if (left <= right)
		{
			for (int y = bottom; y >= top; --y)
				_final_positions[++i % (_n + 1)] = Pos(left, y);
			left++;
		}
	}
}

// Check if positions is valid within the board's coordinates
bool		Square::check_pos(const Pos &pos) const
{
	if (pos.x >= _len_side)
		return (false);
	if (pos.y >= _len_side)
		return (false);
	return (true);
}

// Prints board and highlights the solved numbers green
void	Square::print_board() const
{
	for (uint x = 0; x < _len_side; ++x)
		std::cout << "\t" << x;
	std::cout << std::endl;
	for (uint y = 0; y < _len_side; ++y)
	{
		std::cout << y << "\t";
		for (uint x = 0; x < _len_side; ++x)
		{
			Pos pos(x, y);
			uint num = get_cnum(pos);
			if (_final_positions[num] == pos)
				std::cout << BG_GREEN;
			std::cout << get_cnum(pos) << RESET << "\t";
		}
		std::cout << std::endl;
	}
}

// Count how many numbers are in the right position
void	Square::check_board()
{
	int count = 0;
	for (uint i = 0; i < _final_positions.size(); ++i)
	{
		if (get_cnum(_final_positions[i]) == i)
			count++;
	}
	std::cout << "Solved tiles: " << count << std::endl;
}

Pos			Square::get_pos(const uint num) const
{
	for (uint y = 0; y < _len_side; ++y)
	{
		for (uint x = 0; x < _len_side; ++x)
		{
			Pos pos = Pos(x, y);
			if (get_cnum(pos) == num)
				return (pos);
		}
	}
	return (Pos(_len_side, _len_side));
}

uint	&Square::get_num(const Pos &pos)
{
	assert(check_pos(pos));
	return (_board[pos.x + pos.y * _len_side]);
}

const uint	&Square::get_cnum(const Pos &pos) const
{
	assert(check_pos(pos));
	return (_board[pos.x + pos.y * _len_side]);
}

void		Square::set_num(const Pos &pos, const uint num)
{
	get_num(pos) = num;
}
