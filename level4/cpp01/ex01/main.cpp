#include "Zombie.hpp"

int	main(void)
{
	Zombie*	horde = zombieHorde(5, "Alex");
	delete[]	horde;
	return (0);
}
