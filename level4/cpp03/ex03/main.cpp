	#include "ClapTrap.hpp"
	#include "ScavTrap.hpp"
	#include "FragTrap.hpp"
	#include "DiamondTrap.hpp"

	int	main(void)
	{
		std::cout << BOLD << "\n=== Basic construction test ===" << RESET << std::endl;
		ClapTrap	ct("Clappy");
		ScavTrap	st("Scavy");
		FragTrap	ft("Fraggy");
		DiamondTrap	dt("Dia");

		std::cout << BOLD << "\n=== DiamondTrap whoAmI() test ===" << RESET << std::endl;
		dt.whoAmI();
		std::cout << std::endl;

		std::cout << BOLD << "DiamondTrap stats:" << RESET << std::endl;
		std::cout << "HP : " << dt.getHitPoints() << std::endl;
		std::cout << "EP : " << dt.getEnergyPoints() << std::endl;
		std::cout << "AD : " << dt.getAttackDamage() << std::endl;
		std::cout << std::endl;

		std::cout << BOLD << "\n=== Attack tests ===" << RESET << std::endl;
		ct.attack("Dummy");
		st.attack("Dummy");
		ft.attack("Dummy");
		dt.attack("Dummy");
		std::cout << std::endl;

		std::cout << BOLD << "\n=== Special ability tests ===" << RESET << std::endl;
		st.guardGate();
		ft.highFivesGuys();
		std::cout << std::endl;

		std::cout << BOLD << "\n=== DiamondTrap energy drain test ===" << RESET << std::endl;
		for (int i = 0; i < 15; ++i)
		{
			int	curEp = dt.getEnergyPoints();
			std::cout << "EP before attack [" << i << "] : " << curEp << std::endl;
			dt.attack("Target");
			std::cout << "EP after  attack [" << i << "] : " << dt.getEnergyPoints() << std::endl;
			std::cout << std::endl;
		}

		std::cout << BOLD << "\n=== HP 0 behavior test ===" << RESET << std::endl;
		std::cout << "\n-- Dia takes big damage --" << std::endl;
		dt.takeDamage(200);
		std::cout << "\n-- Dia tries to attack and heal after being defeated --" << std::endl;
		dt.attack("Afterlife");
		dt.beRepaired(50);
		std::cout << std::endl;

		std::cout << BOLD << "\n=== Copy constructor / assignment tests ===" << RESET << std::endl;
		std::cout << "\n-- Copy construct from DiamondTrap dt --" << std::endl;
		DiamondTrap	copy(dt);
		copy.whoAmI();
		std::cout << "Copy HP : " << copy.getHitPoints()
				  << ", EP : " << copy.getEnergyPoints()
				  << ", AD : " << copy.getAttackDamage() << std::endl;
		std::cout << std::endl;

		std::cout << "\n-- Copy assignment to another DiamondTrap --" << std::endl;
		DiamondTrap	another("Another");
		another = dt;
		another.whoAmI();
		std::cout << "Another HP : " << another.getHitPoints()
				  << ", EP : " << another.getEnergyPoints()
				  << ", AD : " << another.getAttackDamage() << std::endl;
		std::cout << std::endl;

		std::cout << BOLD << "\n=== Destruction order test (end of scope) ===" << RESET << std::endl;
		std::cout << "(Destructors will be called automatically now)\n" << std::endl;

		return (0);
	}
