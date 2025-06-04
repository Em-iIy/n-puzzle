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

Square	&Square::operator=(const Square &rhs)
{
	if (*this == rhs)
		return (*this);

	_n = rhs._n;
	_len_side = rhs._len_side;
	_0 = rhs._0;
	_board = rhs._board;
	_final_positions = rhs._final_positions;

	return (*this);
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

int			Square::hamming_distance() const
{
	int count = 0;
	for (uint i = 1; i < _final_positions->size(); ++i)
	{
		if (get_cnum((*_final_positions)[i]) != i)
			count++;
	}
	return (count);
}

int			Square::manhattan_distance() const {
	int count = 0;
	for (uint i = 1; i <= _n; ++i)
	{
		const Pos curr = get_pos(i);
		const Pos goal = (*_final_positions)[i];

		const int curr_x = static_cast<int>(curr.x);
		const int curr_y = static_cast<int>(curr.y);
		const int goal_x = static_cast<int>(goal.x);
		const int goal_y = static_cast<int>(goal.y);

		const int distance = std::abs(curr_x - goal_x) + std::abs(curr_y - goal_y);
		count += distance;
	}
	return count;
}

int Square::linear_conflict() const
{
	int conflicts = 0;

	// Row conflicts
	for (uint row = 0; row < _len_side; ++row)
	{
		for (uint i = 0; i < _len_side; ++i)
		{
			uint tile1 = get_cnum(Pos(i, row));
			if (tile1 == 0 || (*_final_positions)[tile1].y != row)
				continue;

			for (uint j = i + 1; j < _len_side; ++j)
			{
				uint tile2 = get_cnum(Pos(j, row));
				if (tile2 == 0 || (*_final_positions)[tile2].y != row)
					continue;

				if ((*_final_positions)[tile1].x > (*_final_positions)[tile2].x)
					++conflicts;
			}
		}
	}

	// Column conflicts
	for (uint col = 0; col < _len_side; ++col)
	{
		for (uint i = 0; i < _len_side; ++i)
		{
			uint tile1 = get_cnum(Pos(col, i));
			if (tile1 == 0 || (*_final_positions)[tile1].x != col)
				continue ;

			for (uint j = i + 1; j < _len_side; ++j)
			{
				uint tile2 = get_cnum(Pos(col, j));
				if (tile2 == 0 || (*_final_positions)[tile2].x != col)
					continue ;

				if ((*_final_positions)[tile1].y > (*_final_positions)[tile2].y)
				{
					++conflicts;
				}
			}
		}
	}

	return (2 * conflicts); // Each conflict adds 2 moves to resolution
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

bool		Square::check_solvable(const Square &goal)
{
	// Check parity of starting state
	int initial_inversions = 0;
	for (uint i = 0; i < _board.size(); ++i)
	{
		if (_board[i] == 0)
			continue ;
		for (uint j = i + 1; j < _board.size(); ++j)
		{
			if (_board[j] == 0)
				continue ;
			if (_board[i] > _board[j])
			{
				initial_inversions++;
			}
		}
	}

	// Check parity of goal state
	int goal_inversions = 0;
	for (uint i = 0; i < goal._board.size(); ++i)
	{
		if (goal._board[i] == 0)
			continue ;
		for (uint j = i + 1; j < goal._board.size(); ++j)
		{
			if (goal._board[j] == 0)
				continue ;
			if (goal._board[i] > goal._board[j])
				goal_inversions++;
		}
	}

	// If both parities match, the square can go from starting state, to the goal state
	if (_len_side % 2 == 1)
		return (initial_inversions % 2 == goal_inversions % 2);
	int diff = _0.y + goal._0.y;
	if (goal_inversions % 2 == 0)
		return (((initial_inversions % 2) + (diff % 2)) % 2 == 0);
	else
		return (!(((initial_inversions % 2) + (diff % 2)) % 2 == 0));
	
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
	for (uint y = 0; y < _len_side; ++y)
	{
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
	uint count = 0;
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

void		Square::set_solved()
{
	for (uint i = 0; i < _final_positions->size(); ++i)
		set_num((*_final_positions)[i], i);
	_0 = get_pos(0);
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
