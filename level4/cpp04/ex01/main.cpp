#include <iostream>
#include "Animal.hpp"
#include "Dog.hpp"
#include "Cat.hpp"

int main()
{
	const int size = 10;
	Animal* animals[size];
	
	std::cout << "======= Create =======" << std::endl;

	for (int i = 0; i < size; i++)
	{
		if (i < size / 2)
			animals[i] = new Dog();
		else
			animals[i] = new Cat();
	}

	std::cout << "======= Sounds =======" << std::endl;

	for (int i = 0; i < size; i++)
		animals[i]->makeSound();

	std::cout << "======= Deleting =======" << std::endl;
	
	for (int i = 0; i < size; i++)
	delete animals[i];
	
	std::cout << "======= Deep copy test =======" << std::endl;

	Cat*	sleepycat = new Cat();
	std::string	idea = "I want to sleep.";

	sleepycat->getBrain().setIdea(10, idea);
	std::cout << "sleepycat idea : " << sleepycat->getBrain().getIdea(10) << std::endl;

	Cat*	newcat = new Cat(*sleepycat);

	std::cout << "newcat idea : " << newcat->getBrain().getIdea(10) << std::endl;

	delete sleepycat;
	delete newcat;

	std::cout << "======= Brain test =======" << std::endl;

	Brain*		brain = new Brain();
	std::string	idea2 = "I want to eat.";
	std::string	idea3 = "I want to sleep.";

	brain->setIdea(4, idea2);
	brain->setIdea(88, idea3);

	for (int i = 0; i < 100; i++)
		std::cout << "i = " << i << " : " << brain->getIdea(i) << std::endl;

	std::cout << "======= Brain error test =======" << std::endl;
	std::cout << brain->getIdea(100) << std::endl;
	brain->setIdea(100, idea);

	delete brain;

	return (0);
}
