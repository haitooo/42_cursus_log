#include "Character.hpp"

Character::Character() : name("")
{
	for (int i = 0; i < 4; i++)
		inventory[i] = NULL;
}

Character::Character(const std::string& name) : name(name)
{
	for (int i = 0; i < 4; i++)
		inventory[i] = NULL;
}

Character::Character(const Character& other) : name(other.name)
{
	for (int i = 0; i < 4; i++)
	{
		if (other.inventory[i] == NULL)
			inventory[i] = NULL;
		else
			inventory[i] = other.inventory[i]->clone();
	}
}

Character&	Character::operator=(const Character& other)
{
	if (this == &other)
		return (*this);

	name = other.name;
	for (int i = 0; i < 4; i++)
	{
		if (inventory[i] != NULL)
		{
			delete inventory[i];
			inventory[i] = NULL;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		if (other.inventory[i] != NULL)
			inventory[i] = other.inventory[i]->clone();
	}
	return (*this);
}

Character::~Character()
{
	for (int i = 0; i < 4; i++)
	{
		if (inventory[i] != NULL)
			delete inventory[i];
	}
}

std::string const & Character::getName() const
{
	return (name);
}

AMateria*	Character::getEquipment(int idx) const
{
	return (inventory[idx]);
}

void	Character::equip(AMateria* m)
{
	if (m == NULL)
		return ;
	for (int i = 0; i < 4; i++)
	{
		if (inventory[i] == NULL)
		{
			inventory[i] = m;
			return ;
		}
	}
}

void	Character::unequip(int idx)
{
	//before unequip, must save materia address.
	//after unequip, must delete unequiped materia.
	if (idx >= 0 && idx < 4 && inventory[idx] != NULL)
		inventory[idx] = NULL;
}

void	Character::use(int idx, ICharacter& target)
{
	if (idx < 0 || idx >= 4 || inventory[idx] == NULL)
		return ;
	inventory[idx]->use(target);
}

int		Character::countEmptySlots() const
{
	int	counter = 0;

	for (int i = 0; i < 4; i++)
	{
		if (inventory[i] == NULL)
			counter++;
	}
	return (counter);
}
