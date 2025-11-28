#include "DiamondTrap.hpp"

DiamondTrap::DiamondTrap(const std::string &newname): ClapTrap(newname + "_clap_name", default_hitPoint, default_energyPoint, default_attackDamage), name(newname)
{
	std::cout << BLUE
			  << "DiamondTrap <" << name << "> : Constructor called."
			  << RESET << std::endl;
}

DiamondTrap::DiamondTrap(const DiamondTrap& other)
	: ClapTrap(other), FragTrap(other), ScavTrap(other)
{
	*this = other;
	std::cout << BLUE
			  << "DiamondTrap <" << name << "> : Copy Constructor called."
			  << RESET << std::endl;
}

DiamondTrap& DiamondTrap::operator=(const DiamondTrap& other)
{
	if (this != &other)
	{
		ClapTrap::operator=(other);
		this->name = other.name;
	}
	std::cout << BLUE
			  << "DiamondTrap <" << name << "> : Copy assignment operator called."
			  << RESET << std::endl;
	return (*this);
}

DiamondTrap::~DiamondTrap()
{
	std::cout << YELLOW
			  << "DiamondTrap <" << name << "> : Destructor called."
			  << RESET << std::endl;
}

void DiamondTrap::whoAmI()
{
	std::cout << BOLD
			  << "DiamondTrap name: " << name
			  << ", ClapTrap name: " << ClapTrap::name
			  << RESET << std::endl;
}
