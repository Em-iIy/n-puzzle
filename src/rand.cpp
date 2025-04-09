#include <random>

namespace erand
{
	static std::mt19937			mt;

	void	seed()
	{
		std::random_device	rd;
		mt = std::mt19937(rd());
	}

	std::uniform_real_distribution<double>	generator(const double f1, const double f2)
	{
		return (std::uniform_real_distribution<double>(f1, f2));
	}
	
	double	rand(std::uniform_real_distribution<double> &gen)
	{
		return (gen(mt));
	}
	
	std::uniform_int_distribution<int>	generator(const int n1, const int n2)
	{
		return (std::uniform_int_distribution<int>(n1, n2));
	}
	
	int	rand(std::uniform_int_distribution<int> &gen)
	{
		return (gen(mt));
	}
}
