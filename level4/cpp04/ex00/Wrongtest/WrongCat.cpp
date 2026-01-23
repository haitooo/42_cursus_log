#include "WrongCat.hpp"

WrongCat::WrongCat() : WrongAnimal("WrongCat")
{
	std::cout	<< CYAN << ITALIC
				<< "WrongCat default constructor called"
				<< RESET
				<< std::endl;
}

WrongCat::WrongCat(const WrongCat& other) : WrongAnimal(other)
{
	std::cout	<< CYAN << ITALIC
				<< "WrongCat copy constructor called"
				<< RESET
				<< std::endl;
}

WrongCat& WrongCat::operator=(const WrongCat& other)
{
	std::cout	<< CYAN << ITALIC
				<< "WrongCat copy assignment operator called"
				<< RESET
				<< std::endl;

	if (this == &other)
		return (*this);

	WrongAnimal::operator=(other);
	return (*this);
}

WrongCat::~WrongCat()
{
	std::cout	<< MAGENTA << ITALIC
				<< "WrongCat destructor called"
				<< RESET
				<< std::endl;
}

void	WrongCat::makeSound() const
{
	std::cout << "Meow" << std::endl;
}
