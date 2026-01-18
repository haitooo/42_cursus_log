#include "WrongDog.hpp"

WrongDog::WrongDog() : WrongAnimal("WrongDog")
{
	std::cout << "WrongDog default constructor called" << std::endl;
}

WrongDog::WrongDog(const WrongDog& other) : WrongAnimal(other)
{
	std::cout << "WrongDog copy constructor called" << std::endl;
}

WrongDog& WrongDog::operator=(const WrongDog& other)
{
	std::cout << "WrongDog copy assignment operator called" << std::endl;
	setType(other.type);
	return (*this);
}

WrongDog::~WrongDog()
{
	std::cout << "WrongDog destructor called" << std::endl;
}

void	WrongDog::makeSound()
{
	std::cout << "Woof" << std::endl;
}