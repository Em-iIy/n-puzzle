#include <iostream>
#include <string>
#include <vector>

#include "Square.hpp"
#include "rand.hpp"
#include "timer.hpp"
#include "Parser.hpp"

int main(const int argc, char *argv[])
{
	erand::seed();
	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " <filename>\n";
		return (1);
	}
	try
	{
		std::vector<std::vector<int>> grid = parse_puzzle(argv[1]);
		int	n = std::stoi(argv[1]);
		if (n < 1)
			throw std::runtime_error("n must be greater than 0");
		Square sq(n);
		sq.check_board();
		sq.print_board();
	}
	catch(const std::exception& e)
	{
		std::cerr<< "ERROR: " << e.what() << '\n';
		return (1);
	}
	
}
