#include <iostream>
#include <algorithm>
#include <cmath>
#include <cassert>

#include "Square.hpp"
#include "timer.hpp"
#include "rand.hpp"
#include "colour.h"

#include <unordered_map>

Square::Square() {}

// Initialize the square with the lenght of the side
Square::Square(uint n, shared_pos_vec final_positions): _n((n * n) - 1), _len_side(n), _final_positions(final_positions)
{
	_board.reserve(_n + 1);
	for (uint i = 0; i < _n + 1; ++i)
	{
		_board.push_back(i);
	}
	shuffle_board();
	// Find position of the 0 piece
	_0 = get_pos(0);
}

// Initialize the square using a vector of unsigned ints
Square::Square(std::vector<uint> &board, shared_pos_vec final_positions): _n(board.size() - 1), _len_side(sqrt(board.size())), _board(board), _final_positions(final_positions)
{
	// Find position of the 0 piece
	_0 = get_pos(0);
}

Square::Square(const Square &src): _n(src._n), _len_side(src._len_side), _0(src._0), _board(src._board), _final_positions(src._final_positions)
{
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

int			Square::hamming_distance()
{
	int count = 0;
	for (uint i = 1; i < _final_positions->size(); ++i)
	{
		if (get_cnum((*_final_positions)[i]) != i)
			count++;
	}
	return (count * 10);
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
shared_pos_vec init_solved_pos(uint len_side)
{
	shared_pos_vec	final_positions = std::make_shared<std::vector<Pos>>(len_side * len_side);
	int									left = 0;
	int									right = len_side - 1;
	int									top = 0;
	int									bottom = len_side - 1;
	int									i = 0;
	uint								n = len_side * len_side;
	
	while (top <= bottom && left <= right)
	{
		for (int x = left; x <= right; ++x)
			(*final_positions)[++i % n] = Pos(x, top);
		top++;
		for (int y = top; y <= bottom; ++y)
			(*final_positions)[++i % n] = Pos(right, y);
		right--;
		if (top <= bottom)
		{
			for (int x = right; x >= left; --x)
				(*final_positions)[++i % n] = Pos(x, bottom);
			bottom--;
		}
		if (left <= right)
		{
			for (int y = bottom; y >= top; --y)
				(*final_positions)[++i % n] = Pos(left, y);
			left++;
		}
	}
	return (final_positions);
}

bool Square::check_solvable() // REWRITE
{
	std::vector<int> remapped;

	// Map: goal position -> flat index
	std::vector<int> goal_indices(_board.size()); // value -> flattened index
	for (int val = 0; val < _final_positions->size(); ++val)
	{
		const Pos& p = (*_final_positions)[val];
		goal_indices[val] = p.y * _len_side + p.x;
	}

	// Remap current board values to their goal indices
	for (int val : _board)
	{
		if (val != 0)
			remapped.push_back(goal_indices[val]);
	}

	// Count inversions in remapped vector
	int inversions = 0;
	for (int i = 0; i < remapped.size(); ++i)
	{
		for (int j = i + 1; j < remapped.size(); ++j)
		{
			if (remapped[i] > remapped[j])
				inversions++;
		}
	}

	// Apply solvability rules
	if (_len_side % 2 != 0)
	{
		// Odd grid: solvable if inversion count is even
		return inversions % 2 == 0;
	}
	else
	{
		// Even grid: solvable if blank row from bottom and inversion parity are opposite
		return (_len_side - _0.y % 2 == 0) != (inversions % 2 == 0);
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
		std::cout << "\t" << BG_BLUE << x << RESET;
	std::cout << std::endl;
	for (uint y = 0; y < _len_side; ++y)
	{
		std::cout << BG_BLUE << y << RESET << "\t";
		for (uint x = 0; x < _len_side; ++x)
		{
			Pos pos(x, y);
			uint num = get_cnum(pos);
			if ((*_final_positions)[num] == pos)
				std::cout << BG_GREEN;
			if (num == 0)
				std::cout << BG_PINK;
			std::cout << get_cnum(pos) << RESET << "\t";
		}
		std::cout << std::endl;
	}
}

// Count how many numbers are in the right position
bool	Square::check_board()
{
	int count = 0;
	for (uint i = 0; i < _final_positions->size(); ++i)
	{
		if (get_cnum((*_final_positions)[i]) == i)
			count++;
	}
	if (count == _n + 1)
		return (true);
	return (false);
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

bool	Square::operator==(const Square &rhs) const
{
	if (_0 != rhs._0)
		return (false);
	return (_board == rhs._board);
}

bool	Square::operator!=(const Square &rhs) const
{
	return (_board != rhs._board);
}
