#include <iostream>
#include "AAnimal.hpp"
#include "Dog.hpp"
#include "Cat.hpp"

int main()
{
	std::cout << "===== Abstract class test =====" << std::endl;

	/*Compile error case*/
	// AAnimal animal;

	const AAnimal* dog = new Dog();
	const AAnimal* cat = new Cat();

	std::cout << "Dog sound: ";
	dog->makeSound();

	std::cout << "Cat sound: ";
	cat->makeSound();

	std::cout << "----- Deleting -----" << std::endl;

	delete dog;
	delete cat;

	return (0);
}
