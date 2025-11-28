#ifndef CLAP_TRAP_HPP
# define CLAP_TRAP_HPP

# include <iostream>
# include <string>

#define BLUE   "\033[34m"
#define GREEN  "\033[32m"
#define RED    "\033[31m"
#define YELLOW "\033[33m"
#define BOLD   "\033[1m"
#define ITALIC "\033[3m"
#define RESET  "\033[0m"

class	ClapTrap
{
	protected:
		std::string		name;
		int				hitPoints;
		int				energyPoints;
		int				attackDamage;
		ClapTrap(const std::string &n, int hp, int ep, int ad);
	public:
		static const int	default_hitPoint;
		static const int	default_energyPoint;
		static const int	default_attackDamage;

		ClapTrap(const std::string &newname = "CTDefault");
		ClapTrap(const ClapTrap& other);
		ClapTrap& operator=(const ClapTrap& other);
		~ClapTrap();

		std::string	getName() const;
		int			getHitPoints() const;
		int			getEnergyPoints() const;
		int			getAttackDamage() const;

		void attack(const std::string& target);
		void takeDamage(unsigned int amount);
		void beRepaired(unsigned int amount);

};

#endif