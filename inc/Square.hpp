#pragma once

#include <memory>
#include <vector>

#include "Pos.hpp"
#include "types.hpp"

enum e_move
{
	MOVE_UP,
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT
};

class Square
{
	public:
		Square();
		Square(uint n, shared_pos_vec final_positions);
		Square(std::vector<uint> &board, shared_pos_vec final_positions);
		Square(const Square &src);
		~Square();

		int			hamming_distance();

		void		shuffle_board();
		bool		check_solvable();
		bool		check_solvable(const std::vector<int>& goal);
		
		bool		make_move(e_move move);
		bool		check_pos(const Pos &pos) const;
		
		bool		check_board();
		void		print_board() const;

		Pos			get_pos(const uint num) const;
		uint		&get_num(const Pos &pos);
		const uint	&get_cnum(const Pos &pos) const;
		void		set_num(const Pos &pos, const uint num);
		
		bool	operator==(const Square &rhs) const;
		bool	operator!=(const Square &rhs) const;
	
	private:
		uint				_n;
		uint				_len_side;
		Pos					_0;
		std::vector<uint>	_board;
		shared_pos_vec		_final_positions;

};

shared_pos_vec init_solved_pos(uint len_side);