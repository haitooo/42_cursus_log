#include "Zombie.hpp"

int	main(void)
{
	Zombie*	zombie = newZombie("Bob");
	zombie->announce();
	delete zombie;

	randomChump("Alice");

	return (0);
}
