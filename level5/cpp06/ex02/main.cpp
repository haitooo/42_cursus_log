#include "identify.hpp"
#include "Base.hpp"
#include "A.hpp"
#include "B.hpp"
#include "C.hpp"
#include "TextFormatter.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

static void	identifyBoth(Base* p)
{
	std::cout << "  pointer   : ";
	identify(p);
	std::cout << "  reference : ";
	identify(*p);
}

int	main()
{
	std::srand(static_cast<unsigned int>(std::time(NULL)));

	std::cout << CYAN << "--- known types ---" << RESET << std::endl;
	{
		A	a;
		B	b;
		C	c;

		std::cout << "A:" << std::endl;
		identifyBoth(&a);
		std::cout << "B:" << std::endl;
		identifyBoth(&b);
		std::cout << "C:" << std::endl;
		identifyBoth(&c);
	}

	std::cout << CYAN << "--- random types from generate() ---" << RESET << std::endl;
	for (int i = 0; i < 5; i++)
	{
		Base*	p = generate();

		std::cout << "#" << i << ":" << std::endl;
		identifyBoth(p);
		delete p;
	}

	std::cout << CYAN << "--- NULL pointer ---" << RESET << std::endl;
	std::cout << "  pointer   : ";
	identify(static_cast<Base*>(NULL));
	return (0);
}
