#include "Cat.hpp"

Cat::Cat() : Animal("Cat")
{
	std::cout	<< CYAN << ITALIC
				<< "Cat default constructor called"
				<< RESET
				<< std::endl;
}

Cat::Cat(const Cat& other) : Animal(other)
{
	std::cout	<< CYAN << ITALIC
				<< "Cat copy constructor called"
				<< RESET
				<< std::endl;
}

Cat& Cat::operator=(const Cat& other)
{
	std::cout	<< CYAN << ITALIC
				<< "Cat copy assignment operator called"
				<< RESET
				<< std::endl;

	if (this == &other)
		return (*this);

	Animal::operator=(other);
	return (*this);
}

Cat::~Cat()
{
	std::cout	<< MAGENTA << ITALIC
				<< "Cat destructor called"
				<< RESET
				<< std::endl;
}

void	Cat::makeSound() const
{
	std::cout << "Meow" << std::endl;
}
