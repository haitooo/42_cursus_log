#include "Fixed.hpp"

Fixed::Fixed() : value(0)
{
}

Fixed::Fixed(const int i)
{
	value = i << fractionalBits;
}

Fixed::Fixed(const float f)
{
	value = static_cast<int>(roundf(f * (1 << fractionalBits)));
}

Fixed::Fixed(const Fixed& other)
{
	*this = other;
}

Fixed& Fixed::operator=(const Fixed& other)
{
	if (this != &other)
		value = other.value;
	return (*this);
}

Fixed::~Fixed()
{
}

int	Fixed::getRawBits(void) const
{
	return (value);
}

void	Fixed::setRawBits(int const raw)
{
	value = raw;
}

float	Fixed::toFloat(void) const
{
	return (static_cast<float>(value) / (1 << Fixed::fractionalBits));
}

int	Fixed::toInt(void) const
{
	return (value >> Fixed::fractionalBits);
}

std::ostream& operator<<(std::ostream& os, const Fixed& rhs)
{
	return (os << rhs.toFloat());
}

bool Fixed::operator>(const Fixed& other) const
{
	return (value > other.value);
}

bool Fixed::operator<(const Fixed& other) const
{
	return (value < other.value);
}

bool Fixed::operator>=(const Fixed& other) const
{
	return (value >= other.value);
}

bool Fixed::operator<=(const Fixed& other) const
{
	return (value <= other.value);
}

bool Fixed::operator==(const Fixed& other) const
{
	return (value == other.value);
}

bool Fixed::operator!=(const Fixed& other) const
{
	return (value != other.value);
}

Fixed Fixed::operator+(const Fixed& other) const
{
	Fixed	result;

	result.value = value + other.value;
	return (result);
}

Fixed Fixed::operator-(const Fixed& other) const
{
	Fixed	result;

	result.value = value - other.value;
	return (result);
}

Fixed Fixed::operator*(const Fixed& other) const
{
	Fixed	result;
	long	tmp;

	tmp = (static_cast<long>(value) * static_cast<long>(other.value));
	result.value = static_cast<int>(tmp >> fractionalBits);
	return (result);
}

Fixed Fixed::operator/(const Fixed& other) const
{
	Fixed	result;
	long	tmp;
	long	a = static_cast<long>(value);
	long	b = static_cast<long>(other.value);

	tmp = ((a << fractionalBits)/ b);
	result.value = static_cast<int>(tmp);
	return (result);
}

Fixed& Fixed::operator++()
{
	value++;
	return (*this);
}

Fixed Fixed::operator++(int)
{
	Fixed	tmp(*this);
	value++;
	return (tmp);
}

Fixed& Fixed::operator--()
{
	value--;
	return (*this);
}

Fixed Fixed::operator--(int)
{
	Fixed	tmp(*this);
	value--;
	return (tmp);
}

Fixed& Fixed::min(Fixed& a, Fixed& b)
{
	return (a <= b ? a : b);
}

const Fixed& Fixed::min(const Fixed& a, const Fixed& b)
{
	return (a <= b ? a : b);
}

Fixed& Fixed::max(Fixed& a, Fixed& b)
{
	return (a >= b ? a : b);
}

const Fixed& Fixed::max(const Fixed& a, const Fixed& b)
{
	return (a >= b ? a : b);
}
