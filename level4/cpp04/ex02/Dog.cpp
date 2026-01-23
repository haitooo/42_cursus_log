#include "Dog.hpp"

Dog::Dog() : AAnimal("Dog"), brain(new Brain())
{
	std::cout	<< CYAN << ITALIC
				<< "Dog default constructor called"
				<< RESET
				<< std::endl;
}

Dog::Dog(const Dog& other) : AAnimal(other), brain(new Brain(*other.brain))
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

	AAnimal::operator=(other);
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
