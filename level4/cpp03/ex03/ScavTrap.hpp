#ifndef SCAV_TRAP_HPP
# define SCAV_TRAP_HPP

# include "ClapTrap.hpp"

#define BLUE   "\033[34m"
#define GREEN  "\033[32m"
#define RED    "\033[31m"
#define YELLOW "\033[33m"
#define BOLD   "\033[1m"
#define ITALIC "\033[3m"
#define RESET  "\033[0m"

class	ScavTrap : virtual public ClapTrap
{
	public:
		static const int	default_hitPoint;
		static const int	default_energyPoint;
		static const int	default_attackDamage;
		ScavTrap(const std::string &newname = "STDefault");
		ScavTrap(const ScavTrap& other);
		ScavTrap& operator=(const ScavTrap& other);
		virtual ~ScavTrap();

		void attack(const std::string& target);
		void guardGate();
};

#endif
