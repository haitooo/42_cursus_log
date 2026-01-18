#include <iostream>
#include "Animal.hpp"
#include "Dog.hpp"
#include "Cat.hpp"
#include "Wrongtest/WrongAnimal.hpp"
#include "Wrongtest/WrongDog.hpp"
#include "Wrongtest/WrongCat.hpp"

int	main(void)
{
	Animal*	a;
	std::string line;

	std::cout << "1.Dog 2.Cat 3.Wrongmode" << std::endl;
	std::cout << "select number : ";
	std::getline(std::cin, line);

	if (line == "1")
	{
		a = new Dog();
	}
	else if (line == "2")
	{
		a = new Cat();
	}
	else if (line == "3")
	{
		std::cout << "----WrongAnimal test mode----" << std::endl;
		WrongAnimal*	wrong_a;
		std::cout << "1.Dog 2.Cat" << std::endl;
		std::cout << "select number : ";
		std::getline(std::cin, line);

		if (line == "1")
		{
			wrong_a = new WrongDog();
		}
		else if (line == "2")
		{
			wrong_a = new WrongCat();
		}
		else
		{
			std::cout << "invalid number" << std::endl;
			return (1);
		}
		std::cout << std::endl;
		wrong_a->makeSound();
		std::cout << std::endl;

		delete wrong_a;
		return (0);
	}
	else
	{
		std::cout << "invalid number" << std::endl;
		return (1);
	}

	std::cout << std::endl;
	a->makeSound();
	std::cout << std::endl;

	delete a;
	return (0);
}