#include "Point.hpp"

Fixed	cross(const Point a, const Point b, const Point p)
{
	Fixed	abx;
	Fixed	aby;
	Fixed	apx;
	Fixed	apy;
	Fixed	result;

	abx = b.getX() - a.getX();
	aby = b.getY() - a.getY();
	apx = p.getX() - a.getX();
	apy = p.getY() - a.getY();
	result = abx * apy - aby * apx;
	return (result);
}

bool	bsp(Point const a, Point const b, Point const c, Point const point)
{
	Fixed	c1 = cross(a, b, point);
	Fixed	c2 = cross(b, c, point);
	Fixed	c3 = cross(c, a, point);

	if ((c1 > 0 && c2 > 0 && c3 > 0) ||
			(c1 < 0 && c2 < 0 && c3 < 0))
		return (true);
	else
		return (false);
}
