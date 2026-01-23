#include "AAnimal.hpp"

AAnimal::AAnimal() : type("AAnimal")
{
	std::cout	<< CYAN << ITALIC
				<< "AAnimal default constructor called" 
				<< RESET
				<< std::endl;
}

AAnimal::AAnimal(std::string type) : type(type)
{
	std::cout	<< CYAN << ITALIC
				<< "AAnimal string constructor called"
				<< RESET
				<< std::endl;
}

AAnimal::AAnimal(const AAnimal& other) : type(other.type)
{
	std::cout	<< CYAN << ITALIC
				<< "AAnimal copy constructor called"
				<< RESET
				<< std::endl;
}

AAnimal& AAnimal::operator=(const AAnimal& other)
{
	std::cout	<< CYAN << ITALIC
				<< "AAnimal copy assignment operator called"
				<< RESET
				<< std::endl;

	if (this == &other)
		return (*this);

	setType(other.type);
	return (*this);
}

AAnimal::~AAnimal()
{
	std::cout	<< MAGENTA << ITALIC
				<< "AAnimal destructor called"
				<< RESET
				<< std::endl;
}

const std::string&	AAnimal::getType() const
{
	return (type);
}

void	AAnimal::setType(const std::string& type)
{
	this->type = type;
}
