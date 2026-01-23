#include "Dog.hpp"

Dog::Dog() : Animal("Dog"), brain(new Brain())
{
	std::cout	<< CYAN << ITALIC
				<< "Dog default constructor called"
				<< RESET
				<< std::endl;
}

Dog::Dog(const Dog& other) : Animal(other), brain(new Brain(*other.brain))
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
	if (!brain)
		brain = new Brain(*other.brain);
	else
		*brain = *other.brain;
	return (*this);
}

Dog::~Dog()
{
	std::cout	<< MAGENTA << ITALIC
				<< "Dog destructor called"
				<< RESET
				<< std::endl;

	delete brain;
}

Brain&	Dog::getBrain() const
{
	return (*brain);
}

void	Dog::makeSound() const
{
	std::cout << "Woof" << std::endl;
}
