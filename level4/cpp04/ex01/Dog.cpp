#include "Dog.hpp"

Dog::Dog() : Animal("Dog")
{
	std::cout	<< CYAN << ITALIC
				<< "Dog default constructor called"
				<< RESET
				<< std::endl;
}

Dog::Dog(const Dog& other) : Animal(other)
{
	std::cout	<< CYAN << ITALIC
				<< "Dog copy constructor called"
				<< RESET
				<< std::endl;
}

Dog& Dog::operator=(const Dog& other)
{
	std::cout	<< CYAN << ITALIC
				<< "Dog copy assignment operator called"
				<< RESET
				<< std::endl;

	if (this == &other)
		return (*this);

	Animal::operator=(other);
	return (*this);
}

Dog::~Dog()
{
	std::cout	<< MAGENTA << ITALIC
				<< "Dog destructor called"
				<< RESET
				<< std::endl;
}

void	Dog::makeSound() const
{
	std::cout << "Woof" << std::endl;
}
