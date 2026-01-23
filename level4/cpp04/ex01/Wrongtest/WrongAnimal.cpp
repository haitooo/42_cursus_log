#include "WrongAnimal.hpp"

WrongAnimal::WrongAnimal() : type("WrongAnimal")
{
	std::cout	<< CYAN << ITALIC
				<< "WrongAnimal default constructor called"
				<< RESET
				<< std::endl;
}

WrongAnimal::WrongAnimal(std::string type) : type(type)
{
	std::cout	<< CYAN << ITALIC
				<< "WrongAnimal string constructor called"
				<< RESET
				<< std::endl;
}

WrongAnimal::WrongAnimal(const WrongAnimal& other) : type(other.type)
{
	std::cout	<< CYAN << ITALIC
				<< "WrongAnimal copy constructor called"
				<< RESET
				<< std::endl;
}

WrongAnimal& WrongAnimal::operator=(const WrongAnimal& other)
{
	std::cout	<< CYAN << ITALIC
				<< "WrongAnimal copy assignment operator called"
				<< RESET
				<< std::endl;

	if (this == &other)
		return (*this);

	setType(other.type);
	return (*this);
}

WrongAnimal::~WrongAnimal()
{
	std::cout	<< MAGENTA << ITALIC
				<< "WrongAnimal destructor called"
				<< RESET
				<< std::endl;
}

void	WrongAnimal::makeSound() const
{
	std::cout << "WrongAnimal sound" << std::endl;
}

const std::string&	WrongAnimal::getType() const
{
	return (type);
}

void	WrongAnimal::setType(const std::string& type)
{
	this->type = type;
}
