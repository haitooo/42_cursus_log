	#include "ClapTrap.hpp"

	int	main(void)
	{
		ClapTrap	a("Alice");
		ClapTrap	b("Bob");

		std::cout << std::endl;

		a.attack("Bob");
		b.takeDamage(a.getAttackDamage());

		std::cout << std::endl;

		b.beRepaired(5);

		std::cout << std::endl;

		for (int i = 0; i < 12; ++i)
		{
			int	tmp = a.getEnergyPoints();
			a.attack("Dummy");
			if (tmp > 0)
				b.takeDamage(0);
			
			std::cout << std::endl;
		}

		std::cout << std::endl;

		b.takeDamage(100);

		std::cout << std::endl;
		
		b.attack("Bob");
		std::cout << std::endl;

		b.beRepaired(10);
		std::cout << std::endl;

		b.takeDamage(10);
		std::cout << std::endl;

		return (0);
	}
