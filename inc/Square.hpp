#pragma once

#include <vector>

#include "Pos.hpp"

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
		Square(uint n);
		Square(std::vector<uint> &board);
		~Square();

		void		shuffle_board();
		void		init_solved_pos();
		
		bool		make_move(e_move move);
		bool		check_pos(const Pos &pos) const;
		
		void		check_board();
		void		print_board() const;

		Pos			get_pos(const uint num) const;
		uint		&get_num(const Pos &pos);
		const uint	&get_cnum(const Pos &pos) const;
		void		set_num(const Pos &pos, const uint num);
		
	private:
		uint				_n;
		uint				_len_side;
		Pos					_0;
		std::vector<uint>	_board;
		std::vector<Pos>	_final_positions;

};
