#include "FragTrap.hpp"

const int	FragTrap::default_hitPoint = 100;
const int	FragTrap::default_energyPoint = 100;
const int	FragTrap::default_attackDamage = 30;

FragTrap::FragTrap(const std::string &newname) : ClapTrap(newname, default_hitPoint, default_energyPoint, default_attackDamage)
{
	std::cout << BLUE
			  << "FragTrap <" << name << "> : Constructor called."
			  << RESET << std::endl;
}

FragTrap::FragTrap(const FragTrap& other) : ClapTrap(other)
{
	*this = other;
	std::cout << BLUE
			  << "FragTrap <" << name << "> : Copy Constructer called."
			  << RESET << std::endl;
}

FragTrap&	FragTrap::operator=(const FragTrap& other)
{
	if (this != &other)
	{
		ClapTrap::operator=(other);
	}
	std::cout << BLUE
			  << "FragTrap <" << name << "> : Copy assignment operator called."
			  << RESET << std::endl;
	return (*this);
}

FragTrap::~FragTrap()
{
	std::cout << YELLOW
			  << "FragTrap <" << name << "> : Destructer called."
			  << RESET << std::endl;
}

void	FragTrap::highFivesGuys(void)
{
	std::cout << GREEN << BOLD
			  << "FragTrap <" << name << "> requests a high five!"
			  << RESET << std::endl;
}
