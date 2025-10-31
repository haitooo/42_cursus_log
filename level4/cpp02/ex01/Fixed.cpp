#include "Fixed.hpp"

Fixed::Fixed() : value(0)
{
	std::cout << "Default constructor called" << std::endl;
}

Fixed::Fixed(const int i)
{
	std::cout << "Int constructor called" << std::endl;
	value = i << fractionalBits;
}

Fixed::Fixed(const float f)
{
	std::cout << "Float constructor called" << std::endl;
	value = static_cast<int>(roundf(f * (1 << fractionalBits)));
}
Fixed::Fixed(const Fixed& other)
{
	std::cout << "Copy constructor called" << std::endl;
	*this = other;
}

Fixed& Fixed::operator=(const Fixed& other)
{
	std::cout << "Copy assignment operator called" << std::endl;
	if (this != &other)
		value = other.getRawBits();
	return (*this);
}

Fixed::~Fixed()
{
	std::cout << "Destructor called" << std::endl;
}

int	Fixed::getRawBits(void) const
{
	return (value);
}

void	Fixed::setRawBits(int const raw)
{
	std::cout << "setRawBits member function called" << std::endl;
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
	os << rhs.toFloat();
	return (os);
}
