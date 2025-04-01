#pragma once

#include <random>

namespace erand
{

	void	seed();

	std::uniform_real_distribution<double>	generator(const double f1, const double f2);

	double	rand(std::uniform_real_distribution<double> &gen);
}
