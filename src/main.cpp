#include <iostream>
#include <string>
#include <vector>

#include "Square.hpp"
#include "rand.hpp"
#include "timer.hpp"

int main(int argc, char *argv[])
{
	erand::seed();
	if (argc != 2)
	{
		std::cerr << "Invalid usage: n-puzzle <n>" << std::endl;
		return (1);
	}
	try
	{
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
