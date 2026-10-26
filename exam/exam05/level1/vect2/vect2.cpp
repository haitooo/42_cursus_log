#include "vect2.hpp"

vect2::vect2() : x(0), y(0)
{
}

vect2::vect2(int n1, int n2) : x(n1), y(n2)
{
}

vect2::vect2(const vect2& other)
{
	if (this != &other)
	{
		this->x = other.x;
		this->y = other.y;
	}
}

vect2& vect2::operator=(const vect2& other)
{
	if (this != &other)
	{
		this->x = other.x;
		this->y = other.y;
	}
	return (*this);
}

vect2::~vect2()
{
}

int vect2::getX() const
{
	return (this->x);
}

int vect2::getY() const
{
	return (this->y);
}

std::ostream& operator<<(std::ostream& os, const vect2& v2)
{
	os << "{" << v2.getX() << ", " << v2.getY() << "}";
	return (os);
}

int	vect2::operator[](int i) const
{
	if (i == 0)
		return(getX());
	return(getY());
}

int&	vect2::operator[](int i)
{
	if (i == 0)
		return(x);
	return(y);
}

vect2&	vect2::operator++()
{
	x++;
	y++;
	return (*this);
}

vect2	vect2::operator++(int)
{
	vect2 tmp(*this);
	++*this;
	return (tmp);
}

vect2&	vect2::operator--()
{
	x--;
	y--;
	return (*this);
}

vect2	vect2::operator--(int)
{
	vect2 tmp(*this);
	--*this;
	return (tmp);
}

vect2	vect2::operator+(const vect2& rhs) const
{
	vect2 new_vect2(x + rhs.getX(), y + rhs.getY());
	return (new_vect2);
}

vect2	vect2::operator+(int n) const
{
	vect2 new_vect2(x + n, y + n);
	return (new_vect2);
}

vect2	vect2::operator+() const
{
	return(*this);
}

vect2	vect2::operator-(const vect2& rhs) const
{
	vect2 new_vect2(x - rhs.getX(), y - rhs.getY());
	return (new_vect2);
}

vect2	vect2::operator-(int n) const
{
	vect2 new_vect2(x - n, y - n);
	return (new_vect2);
}

vect2	vect2::operator-() const
{
	vect2 new_vect2(-x, -y);
	return (new_vect2);
}

vect2	vect2::operator*(const vect2& rhs) const
{
	vect2 new_vect2(x * rhs.getX(), y * rhs.getY());
	return (new_vect2);
}

vect2	vect2::operator*(int n) const
{
	vect2 new_vect2(x * n, y * n);
	return (new_vect2);
}

vect2&	vect2::operator+=(const vect2& rhs)
{
	x += rhs.getX();
	y += rhs.getY();
	return (*this);
}

vect2&	vect2::operator+=(int n)
{
	x += n;
	y += n;
	return (*this);
}

vect2&	vect2::operator-=(const vect2& rhs)
{
	x -= rhs.getX();
	y -= rhs.getY();
	return (*this);
}

vect2&	vect2::operator-=(int n)
{
	x -= n;
	y -= n;
	return (*this);
}

vect2&	vect2::operator*=(const vect2& rhs)
{
	x *= rhs.getX();
	y *= rhs.getY();
	return (*this);
}

vect2&	vect2::operator*=(int n)
{
	x *= n;
	y *= n;
	return (*this);
}

bool	vect2::operator==(const vect2& rhs) const
{
	if (x == rhs.getX() && y == rhs.getY())
		return (true);
	return (false);
}

bool	vect2::operator!=(const vect2& rhs) const
{
	if (x != rhs.getX() || y != rhs.getY())
		return (true);
	return (false);
}

vect2 operator+(int i, const vect2& v2)
{
	vect2 new_vect2(i + v2.getX(), i + v2.getY());
	return (new_vect2);
}

vect2 operator-(int i, const vect2& v2)
{
	vect2 new_vect2(i - v2.getX(), i - v2.getY());
	return (new_vect2);
}

vect2 operator*(int i, const vect2& v2)
{
	vect2 new_vect2(i * v2.getX(), i * v2.getY());
	return (new_vect2);
}

