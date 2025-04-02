#pragma once

#include <random>

namespace erand
{

	void	seed();

	std::uniform_real_distribution<double>	generator(const double f1, const double f2);

	double	rand(std::uniform_real_distribution<double> &gen);

	std::uniform_int_distribution<int>	generator(const int n1, const int n2);

	int	rand(std::uniform_int_distribution<int> &gen);
}
