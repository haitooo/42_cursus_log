#ifndef FRAG_TRAP_HPP
# define FRAG_TRAP_HPP

# include "ClapTrap.hpp"

#define BLUE   "\033[34m"
#define GREEN  "\033[32m"
#define RED    "\033[31m"
#define YELLOW "\033[33m"
#define BOLD   "\033[1m"
#define ITALIC "\033[3m"
#define RESET  "\033[0m"

class	FragTrap : public ClapTrap
{
	public:
		static const int	default_hitPoint;
		static const int	default_energyPoint;
		static const int	default_attackDamage;
		FragTrap(const std::string &newname = "FTDefault");
		FragTrap(const FragTrap& other);
		FragTrap&	operator=(const FragTrap& other);
		~FragTrap();

		void		highFivesGuys(void);
};

#endif
