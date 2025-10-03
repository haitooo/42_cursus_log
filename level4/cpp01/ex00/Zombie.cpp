#include "Zombie.hpp"

Zombie::Zombie() : name()
{
}

Zombie::Zombie(std::string name)
{
	this->name = name;
}
		
Zombie::~Zombie()
{
	std::cout << name << " is deleted." << std::endl;
}

void	Zombie::announce()
{
	std::cout << name << ": BraiiiiiiinnnzzzZ..." << std::endl;
}
