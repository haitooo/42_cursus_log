#include "Cat.hpp"

Cat::Cat() : AAnimal("Cat"), brain(new Brain())
{
	std::cout	<< CYAN << ITALIC
				<< "Cat default constructor called"
				<< RESET
				<< std::endl;
}

Cat::Cat(const Cat& other) : AAnimal(other), brain(new Brain(*other.brain))
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

	AAnimal::operator=(other);
	if (!brain)
		brain = new Brain(*other.brain);
	else
		*brain = *other.brain;
	return (*this);
}

Cat::~Cat()
{
	std::cout	<< MAGENTA << ITALIC
				<< "Cat destructor called"
				<< RESET
				<< std::endl;

	delete brain;
}

Brain&	Cat::getBrain() const
{
	return (*brain);
}

void	Cat::makeSound() const
{
	std::cout << "Meow" << std::endl;
}
