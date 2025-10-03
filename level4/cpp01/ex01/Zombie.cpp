#include "Zombie.hpp"

Zombie::Zombie() : name()
{
}

Zombie::Zombie(std::string const& name)
{
	this->name = name;
}
		
Zombie::~Zombie()
{
	std::cout << name << " is deleted." << std::endl;
}

void	Zombie::announce() const
{
	std::cout << name << ": BraiiiiiiinnnzzzZ..." << std::endl;
}

void	Zombie::setName(std::string const& newName)
{
	name = newName;
}
