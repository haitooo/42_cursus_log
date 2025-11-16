#include <cstdlib>
#include <cerrno>
#include "Point.hpp"

static bool	parseFloat(const char* str, float& out)
{
	if (!str || *str == '\0')
		return (false);

	char*	endptr = NULL;
	errno	= 0;
	double	val = std::strtod(str, &endptr);

	if (endptr == str || *endptr != '\0' || errno == ERANGE)
		return (false);
	out = static_cast<float>(val);
	return (true);
}

int	printUsage()
{
	std::cout << "Usage:\n" << "  ./bsp ax ay bx by cx cy px py (value is int or float)" << std::endl;
	return (1);
}

int	main(int argc, char** argv)
{
	float	vals[8];

	if (argc != 9)
		return (printUsage());
	for (int i = 0; i < 8; ++i)
	{
		if (!parseFloat(argv[i + 1], vals[i]))
			return (printUsage());
	}

	Point a(vals[0], vals[1]);
	Point b(vals[2], vals[3]);
	Point c(vals[4], vals[5]);
	Point p(vals[6], vals[7]);

	if (bsp(a, b, c, p))
		std::cout << "point is inside triangle." << std::endl;
	else
		std::cout << "point is outside triangle." << std::endl;

	return (0);
}
