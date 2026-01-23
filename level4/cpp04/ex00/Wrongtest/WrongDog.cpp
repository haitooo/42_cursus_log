#include "WrongDog.hpp"

WrongDog::WrongDog() : WrongAnimal("WrongDog")
{
	std::cout	<< CYAN << ITALIC
				<< "WrongDog default constructor called"
				<< RESET
				<< std::endl;
}

WrongDog::WrongDog(const WrongDog& other) : WrongAnimal(other)
{
	std::cout	<< CYAN << ITALIC
				<< "WrongDog copy constructor called"
				<< RESET
				<< std::endl;
}

WrongDog& WrongDog::operator=(const WrongDog& other)
{
	std::cout	<< CYAN << ITALIC
				<< "WrongDog copy assignment operator called"
				<< RESET
				<< std::endl;

	if (this == &other)
		return (*this);

	WrongAnimal::operator=(other);
	return (*this);
}

WrongDog::~WrongDog()
{
	std::cout	<< MAGENTA << ITALIC
				<< "WrongDog destructor called"
				<< RESET
				<< std::endl;
}

void	WrongDog::makeSound() const
{
	std::cout << "Woof" << std::endl;
}
