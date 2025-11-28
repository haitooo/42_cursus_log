#include "ClapTrap.hpp"

const int	ClapTrap::default_hitPoint = 10;
const int	ClapTrap::default_energyPoint = 10;
const int	ClapTrap::default_attackDamage = 0;

ClapTrap::ClapTrap(const std::string &n, int hp, int ep, int ad) :
					name(n), hitPoints(hp), energyPoints(ep), attackDamage(ad)
{
	std::cout << BLUE
			  << "ClapTrap <" << name << "> : Constructer called."
			  << RESET << std::endl;
}

ClapTrap::ClapTrap(const std::string &newname) :
					name(newname), hitPoints(default_hitPoint), energyPoints(default_energyPoint), attackDamage(default_attackDamage)
{
	std::cout << BLUE
			  << "ClapTrap <" << name << "> : Constructer called."
			  << RESET << std::endl;
}

ClapTrap::ClapTrap(const ClapTrap& other) : name(other.name), hitPoints(other.hitPoints),
											energyPoints(other.energyPoints), attackDamage(other.attackDamage)
{
	std::cout << BLUE
			  << "ClapTrap <" << name << "> : Copy Constructer called."
			  << RESET << std::endl;
}

ClapTrap&	ClapTrap::operator=(const ClapTrap& other)
{
	if (this != &other)
	{
		name = other.name;
		hitPoints = other.hitPoints;
		energyPoints = other.energyPoints;
		attackDamage = other.attackDamage;
	}
	std::cout << BLUE
			  << "ClapTrap <" << name << "> : Copy assignment operator called."
			  << RESET << std::endl;
	return (*this);
}

ClapTrap::~ClapTrap()
{
	std::cout << YELLOW
			  << "ClapTrap <" << name << "> : Destructer called."
			  << RESET << std::endl;
}

std::string	ClapTrap::getName() const
{
	return (name);
}

int	ClapTrap::getHitPoints() const
{
	return (hitPoints);
}

int	ClapTrap::getEnergyPoints() const
{
	return (energyPoints);
}

int	ClapTrap::getAttackDamage() const
{
	return (attackDamage);
}

void	ClapTrap::attack(const std::string& target)
{
	std::cout << BOLD
			  << "[ATTACK] ClapTrap " << name << " → " << target << " : " << attackDamage << " DMG"
			  << RESET << std::endl;
	if (hitPoints <= 0)
	{
		std::cout << BOLD
				  << "ClapTrap " << name << " tries to attack, but is already defeated..."
				  << RESET << std::endl;
		return ;
	}
	if (energyPoints <= 0)
	{
		std::cout << BOLD
				  << "ClapTrap " << name << "'s energy is depleted!"
				  << RESET << std::endl;
		return ;
	}
	energyPoints--;
}

void	ClapTrap::takeDamage(unsigned int amount)
{
	if (hitPoints <= 0)
	{
		std::cout << BOLD
				  << "ClapTrap " << name << " is already defeated..."
				  << RESET << std::endl;
		return ;
	}
	hitPoints -= amount;
	std::cout << BOLD
			  << "ClapTrap " << name << " takes " << amount << " damage!!"
			  << RESET << std::endl;
	if (hitPoints <= 0)
	{
		std::cout << BOLD
				  << "ClapTrap " << name << " is wiped out!"
				  << RESET << std::endl;
	}
}

void	ClapTrap::beRepaired(unsigned int amount)
{
	std::cout << BOLD
			  << "[HEAL]   ClapTrap " << name << " : " << amount << " POINT"
			  << RESET << std::endl;
	if (hitPoints <= 0)
	{
		std::cout << BOLD
				  << "ClapTrap " << name << " tries to heal, but is already defeated..."
				  << RESET << std::endl;
		return ;
	}
	if (energyPoints <= 0)
	{
		std::cout << BOLD
				  << "ClapTrap " << name << "'s energy is depleted!"
				  << RESET << std::endl;
		return ;
	}
	energyPoints--;
	hitPoints += amount;
	std::cout << BOLD
			  << "ClapTrap " << name << " heals " << amount << " hit points."
			  << RESET << std::endl;
}
