#ifndef HUMAN_B
# define HUMAN_B

# include <string>
# include <iostream>
# include "Weapon.hpp"

class HumanB
{
	private:
		std::string	name;
		Weapon*		weapon;

	public:
		HumanB(const std::string& name);
		~HumanB();
		void	attack() const;
		void	setWeapon(Weapon& newWeapon);
};

#endif
