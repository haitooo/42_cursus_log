#include "FragTrap.hpp"

FragTrap::FragTrap(std::string newname) : ClapTrap(newname)
{
	std::cout << BLUE
			  << "FragTrap <" << name << "> : Constructor called."
			  << RESET << std::endl;

	hitPoints = 100;
	energyPoints = 100;
	attackDamage = 30;
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

void	FragTrap::attack(const std::string& target)
{
	std::cout << BOLD
			  << "[ATTACK] FragTrap " << name << " → " << target
			  << " : " << attackDamage << " DMG"
			  << RESET << std::endl;
	if (hitPoints <= 0)
	{
		std::cout << BOLD
				  << "FragTrap " << name << " tries to attack, but is already defeated..."
				  << RESET << std::endl;
		return ;
	}
	if (energyPoints <= 0)
	{
		std::cout << BOLD
				  << "FragTrap " << name << "'s energy is depleted!"
				  << RESET << std::endl;
		return ;
	}
	energyPoints--;
}

void	FragTrap::highFivesGuys(void)
{
	std::cout << GREEN << BOLD
			  << "FragTrap <" << name << "> requests a high five!"
			  << RESET << std::endl;
}
