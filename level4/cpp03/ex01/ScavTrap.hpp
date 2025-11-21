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

class	ScavTrap : public ClapTrap
{
	public:
		ScavTrap(std::string newname);
		ScavTrap(const ScavTrap& other);
		ScavTrap& operator=(const ScavTrap& other);
		~ScavTrap();

		void attack(const std::string& target);
		void guardGate();
};

#endif
