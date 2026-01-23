#include "Brain.hpp"

Brain::Brain()
{
	std::cout	<< CYAN << ITALIC
				<< "Brain default constructor called" 
				<< RESET
				<< std::endl;
}

Brain::Brain(const Brain& other)
{
	std::cout	<< CYAN << ITALIC
				<< "Brain copy constructor called"
				<< RESET
				<< std::endl;

	for (int i = 0; i < 100; i++)
		ideas[i] = other.ideas[i];
}

Brain& Brain::operator=(const Brain& other)
{
	std::cout	<< CYAN << ITALIC
				<< "Brain copy assignment operator called"
				<< RESET
				<< std::endl;

	if (this == &other)
		return (*this);

	for (int i = 0; i < 100; i++)
		ideas[i] = other.ideas[i];
	return (*this);
}

Brain::~Brain()
{
	std::cout	<< MAGENTA << ITALIC
				<< "Brain destructor called"
				<< RESET
				<< std::endl;
}

const std::string&	Brain::getIdea(int index) const
{
	static const std::string	empty;
	if (index < 0 || index >= 100)
	{
		std::cout	<< BOLD << RED << "getIdea : <Error> Invalid index." << RESET
					<< std::endl;
		return (empty);
	}
	return (ideas[index]);
}

void	Brain::setIdea(int index, const std::string& idea)
{
	if (index < 0 || index > 99)
	{
		std::cout	<< BOLD << RED << "setIdea : <Error> Invalid index." << RESET
					<< std::endl;
		return ;
	}
	ideas[index] = idea;
}
