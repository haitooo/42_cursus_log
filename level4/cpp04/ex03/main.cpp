#include "MateriaSource.hpp"
#include "Ice.hpp"
#include "Cure.hpp"
#include "Character.hpp"
#include <iostream>
#include <string>

int main()
{
	//===== subject example test =====
	IMateriaSource* src = new MateriaSource();
	src->learnMateria(new Ice());
	src->learnMateria(new Cure());

	ICharacter* me = new Character("me");

	AMateria* tmp;
	tmp = src->createMateria("ice");
	me->equip(tmp);
	tmp = src->createMateria("cure");
	me->equip(tmp);

	ICharacter* bob = new Character("bob");
	me->use(0, *bob);
	me->use(1, *bob);

	delete bob;
	delete me;
	delete src;

	//===== learnMateria memory leak test =====
	MateriaSource*	ms = new MateriaSource();

	ms->learnMateria(new Ice());
	for (int i = 0; i < 10; i++)
		ms->learnMateria(new Cure());

	delete ms;

	//===== equip memory leak test =====
	Character*	knight = new Character("knight");

	IMateriaSource* ms2 = new MateriaSource();
	ms2->learnMateria(new Ice());
	for (int i = 0; i < 10; i++)
	{
		AMateria* tmp2;
		tmp2 = ms2->createMateria("ice");
		if (knight->countEmptySlots() != 0)
			knight->equip(tmp2);
		else
			delete tmp2;
	}

	delete ms2;
	delete knight;

	//===== unequip memory leak test =====
	Character*	knight3 = new Character("knight");

	IMateriaSource* ms3 = new MateriaSource();
	ms3->learnMateria(new Ice());
	for (int i = 0; i < 4; i++)
	{
		AMateria* tmp3;
		tmp3 = ms3->createMateria("ice");
		if (knight3->countEmptySlots() != 0)
			knight3->equip(tmp3);
		else
			delete tmp3;
	}
	/*--How to use unequip--*/
	AMateria*	dropMateria;
	dropMateria = knight3->getEquipment(0);
	knight3->unequip(0);
	delete dropMateria;
	/*----------------------*/

	delete ms3;
	delete knight3;

	return (0);
}
