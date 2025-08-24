#ifndef HUMAN_A
# define HUMAN_A

# include <string>
# include <iostream>
# include "Weapon.hpp"

class HumanA
{
	private:
		std::string	name;
		Weapon		&weapon;

	public:
		HumanA(const std::string &name, Weapon &weapon);
		void	attack() const;
};

void	attack();

#endif