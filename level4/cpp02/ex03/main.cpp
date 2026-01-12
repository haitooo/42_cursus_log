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

	// Check if a, b, c form a valid triangle
	Fixed	ab_x = b.getX() - a.getX();
	Fixed	ab_y = b.getY() - a.getY();
	Fixed	ac_x = c.getX() - a.getX();
	Fixed	ac_y = c.getY() - a.getY();
	Fixed	cross_product = ab_x * ac_y - ab_y * ac_x;

	if (cross_product == 0)
	{
		std::cout << "Error: The three points are collinear. They do not form a valid triangle." << std::endl;
		return (1);
	}

	if (bsp(a, b, c, p))
		std::cout << "point is inside triangle." << std::endl;
	else
		std::cout << "point is outside triangle." << std::endl;

	return (0);
}

// ./bsp 0 0 5 0 0 5 1 1
// ./bsp 0 0 5 0 0 5 5 5
// ./bsp 0 0 5 0 0 5 0 0
// ./bsp 0 0 5 0 0 5 -1 -1