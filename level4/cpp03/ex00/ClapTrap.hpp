#ifndef CLAP_TRAP_HPP
# define CLAP_TRAP_HPP

# include <iostream>
# include <string>
# include "TextFormatter.h"

class	ClapTrap
{
	private:
		std::string		name;
		unsigned int	hitPoints;
		unsigned int	energyPoints;
		unsigned int	attackDamage;
		ClapTrap(const std::string &n, unsigned int hp, unsigned int ep, unsigned int ad);

	public:
		static const unsigned int	default_hitPoint;
		static const unsigned int	default_energyPoint;
		static const unsigned int	default_attackDamage;

		ClapTrap(const std::string &newname = "CTDefault");
		ClapTrap(const ClapTrap& other);
		ClapTrap& operator=(const ClapTrap& other);
		~ClapTrap();

		std::string		getName() const;
		unsigned int	getHitPoints() const;
		unsigned int	getEnergyPoints() const;
		unsigned int	getAttackDamage() const;

		void attack(const std::string& target);
		void takeDamage(unsigned int amount);
		void beRepaired(unsigned int amount);
};

#endif
