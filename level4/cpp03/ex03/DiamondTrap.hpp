#ifndef DIAMOND_TRAP_HPP
#define DIAMOND_TRAP_HPP

#include "ScavTrap.hpp"
#include "FragTrap.hpp"

class	DiamondTrap : public FragTrap, public ScavTrap
{
	private:
		std::string	name;
	public:
		using FragTrap::default_hitPoint;
		using ScavTrap::default_energyPoint;
		using FragTrap::default_attackDamage;

		DiamondTrap(const std::string &newname = "DTDefault");
		DiamondTrap(const DiamondTrap& other);
		DiamondTrap& operator=(const DiamondTrap& other);
		~DiamondTrap();

		using	ScavTrap::attack;
		void	whoAmI(void);
};

#endif
