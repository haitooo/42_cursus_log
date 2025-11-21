	#include "FragTrap.hpp"

	int	main(void)
	{
		ClapTrap	ct1("Alice");
		ClapTrap	ct2("Bob");

		std::cout << std::endl;

		ct1.attack("Bob");
		ct2.takeDamage(ct1.getAttackDamage());

		std::cout << std::endl;

		ct2.beRepaired(5);

		std::cout << std::endl;

		FragTrap	st1("Fragy");
		FragTrap	st2("Highter");

		std::cout << std::endl;

		st1.attack("Highter");
		st2.takeDamage(st1.getAttackDamage());

		std::cout << std::endl;

		st2.highFivesGuys();

		std::cout << std::endl;

		for (int i = 0; i < 102; ++i)
		{
			int	tmp = st1.getEnergyPoints();
			st1.attack("Dummy");
			if (tmp > 0)
				st2.takeDamage(0);
			std::cout << std::endl;
		}

		std::cout << std::endl;

		st2.takeDamage(150);
		std::cout << std::endl;

		st2.attack("Dummy");
		std::cout << std::endl;

		st2.beRepaired(10);
		std::cout << std::endl;

		FragTrap	st3(st1);
		std::cout << std::endl;

		FragTrap	st4("Temp");
		st4 = st2;
		std::cout << std::endl;

		return (0);
	}
