#include "ScavTrap.hpp"

ScavTrap::ScavTrap(std::string newname) : ClapTrap(newname)
{
	std::cout << BLUE
			  << "ScavTrap <" << name << "> : Constructor called."
			  << RESET << std::endl;

	this->hitPoints = 100;
	this->energyPoints = 50;
	this->attackDamage = 20;
}

ScavTrap::ScavTrap(const ScavTrap& other) : ClapTrap(other)
{
	*this = other;
	std::cout << BLUE
			  << "ScavTrap <" << name << "> : Copy Constructer called."
			  << RESET << std::endl;
}

ScavTrap&	ScavTrap::operator=(const ScavTrap& other)
{
	if (this != &other)
	{
		ClapTrap::operator=(other);
	}
	std::cout << BLUE
			  << "ScavTrap <" << name << "> : Copy assignment operator called."
			  << RESET << std::endl;
	return (*this);
}

ScavTrap::~ScavTrap()
{
	std::cout << YELLOW
			  << "ScavTrap <" << name << "> : Destructer called."
			  << RESET << std::endl;
}

void	ScavTrap::attack(const std::string& target)
{
	std::cout << BOLD
			  << "[ATTACK] ScavTrap " << name << " → " << target << " : " << attackDamage << " DMG"
			  << RESET << std::endl;
	if (hitPoints <= 0)
	{
		std::cout << BOLD
				  << "ScavTrap " << name << " tries to attack, but is already defeated..."
				  << RESET << std::endl;
		return ;
	}
	if (energyPoints <= 0)
	{
		std::cout << BOLD
				  << "ScavTrap " << name << "'s energy is depleted!"
				  << RESET << std::endl;
		return ;
	}
	energyPoints--;
}

void	ScavTrap::guardGate()
{
	std::cout << GREEN << BOLD
			  << "ScavTrap <" << name << "> has entered Gate Keeper Mode!"
			  << RESET << std::endl;
}
