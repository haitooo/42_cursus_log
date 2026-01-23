#include <iostream>
#include "Animal.hpp"
#include "Dog.hpp"
#include "Cat.hpp"
#include "Wrongtest/WrongAnimal.hpp"
#include "Wrongtest/WrongDog.hpp"
#include "Wrongtest/WrongCat.hpp"
#include "TextFormatter.h"

int	main(void)
{
	Animal*	a;
	std::string line;

	std::cout << BOLD << GREEN << "1.Dog 2.Cat 3.Wrongmode" <<  RESET <<std::endl;
	std::cout << BOLD << BLINK << GREEN <<"select number : " << RESET;
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
		WrongAnimal*	wrong_a;

		std::cout << BOLD << YELLOW << "==== WrongAnimal test mode ====" << RESET << std::endl;
		std::cout << BOLD << GREEN << "1.Dog 2.Cat" << RESET << std::endl;
		std::cout << BOLD << BLINK << GREEN << "select number : " << RESET;
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
			std::cout << BOLD << RED << "invalid number" << RESET << std::endl;
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
		std::cout << BOLD << RED << "invalid number" << RESET << std::endl;
		return (1);
	}

	std::cout << std::endl;
	a->makeSound();
	std::cout << std::endl;

	delete a;
	return (0);
}

// ===========copy assignment test =============

// #include <iostream>
// #include "Animal.hpp"
// #include "Cat.hpp"
// #include "Dog.hpp"

// int main()
// {
//     std::cout << "=== Create objects ===" << std::endl;
//     Cat cat1;
//     Cat cat2;
//     // Dog dog1;
//     // Dog dog2;


// 	cat1.setType("test");
// 	std::cout << "cat1 : " << cat1.getType() << std::endl;
// 	std::cout << "cat2 : " << cat2.getType() << std::endl;
//     std::cout << "\n=== Cat copy assignment ===" << std::endl;
//     cat2 = cat1;
	
// 	std::cout << "cat1 : " << cat1.getType() << std::endl;
// 	std::cout << "cat2 : " << cat2.getType() << std::endl;

//     // std::cout << "\n=== Cat self assignment ===" << std::endl;
//     // cat1 = cat1;

//     // std::cout << "\n=== Dog copy assignment ===" << std::endl;
//     // dog2 = dog1;

//     // std::cout << "\n=== Dog self assignment ===" << std::endl;
//     // dog1 = dog1;

//     // std::cout << "\n=== Polymorphic assignment via base reference ===" << std::endl;
//     // Animal& a1 = cat1;
//     // Animal& a2 = cat2;

//     // a1 = a2;

//     std::cout << "\n=== End of main ===" << std::endl;
//     return 0;
// }
