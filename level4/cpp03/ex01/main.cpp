	#include "ClapTrap.hpp"
	#include "ScavTrap.hpp"

	int	main(void)
	{
		// 基本動作テスト（ClapTrap）
		ClapTrap	ct1("Alice");
		ClapTrap	ct2("Bob");

		std::cout << std::endl;

		ct1.attack("Bob");
		ct2.takeDamage(ct1.getAttackDamage());

		std::cout << std::endl;

		ct2.beRepaired(5);

		std::cout << std::endl;

		// ScavTrap の基本動作テスト
		ScavTrap	st1("Scavy");
		ScavTrap	st2("Guardian");

		std::cout << std::endl;

		st1.attack("Guardian");
		st2.takeDamage(st1.getAttackDamage());

		std::cout << std::endl;

		// Gate Keeper モードへの移行テスト
		st2.guardGate();

		std::cout << std::endl;

		// ScavTrap のエネルギー切れテスト
		for (int i = 0; i < 52; ++i)
		{
			int	tmp = st1.getEnergyPoints();
			st1.attack("Dummy");
			if (tmp > 0)
				st2.takeDamage(0);
			std::cout << std::endl;
		}

		std::cout << std::endl;

		// 倒れた ScavTrap にさらに攻撃・回復させてみる
		st2.takeDamage(150);
		std::cout << std::endl;

		st2.attack("Dummy");
		std::cout << std::endl;

		st2.beRepaired(10);
		std::cout << std::endl;

		// コピーコンストラクタと代入演算子のテスト
		ScavTrap	st3(st1);     // copy constructor
		std::cout << std::endl;

		ScavTrap	st4("Temp");
		st4 = st2;               // copy assignment
		std::cout << std::endl;

		return (0);
	}
