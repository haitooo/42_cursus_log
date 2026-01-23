#include "Animal.hpp"

Animal::Animal() : type("Animal")
{
	std::cout	<< CYAN << ITALIC
				<< "Animal default constructor called" 
				<< RESET
				<< std::endl;
}

Animal::Animal(std::string type) : type(type)
{
	std::cout	<< CYAN << ITALIC
				<< "Animal string constructor called"
				<< RESET
				<< std::endl;
}

Animal::Animal(const Animal& other) : type(other.type)
{
	std::cout	<< CYAN << ITALIC
				<< "Animal copy constructor called"
				<< RESET
				<< std::endl;
}

Animal& Animal::operator=(const Animal& other)
{
	std::cout	<< CYAN << ITALIC
				<< "Animal copy assignment operator called"
				<< RESET
				<< std::endl;

	if (this == &other)
		return (*this);

	setType(other.type);
	return (*this);
}

Animal::~Animal()
{
	std::cout	<< MAGENTA << ITALIC
				<< "Animal destructor called"
				<< RESET
				<< std::endl;
}

void	Animal::makeSound() const
{
	std::cout << "Animal sound" << std::endl;
}

std::string	Animal::getType() const
{
	return (type);
}

void	Animal::setType(std::string type)
{
	this->type = type;
}
