#include "Array.hpp"
#include "TextFormatter.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

template <typename T>
void	printArray(std::string const & label, Array<T> const & array)
{
	std::cout << label << " (size " << array.size() << "): ";
	for (unsigned int i = 0; i < array.size(); i++)
		std::cout << "[" << array[i] << "] ";
	std::cout << std::endl;
}

void	showDefaultInit()
{
	std::cout << YELLOW << "---default initialization---" << RESET << std::endl;

	Array<int>			ints(5);
	Array<std::string>	strs(3);

	printArray("Array<int>(5)", ints);
	printArray("Array<std::string>(3)", strs);
}

void	showDeepCopy()
{
	std::cout << YELLOW << "---deep copy test---" << RESET << std::endl;

	Array<std::string>	original(3);

	original[0] = "foo";
	original[1] = "bar";
	original[2] = "baz";

	Array<std::string>	copied(original);

	Array<std::string>	assigned;
	assigned = original;

	original[0] = "CHANGED";
	copied[1] = "COPY";
	assigned[2] = "ASSIGN";
	printArray("original", original);
	printArray("copied  ", copied);
	printArray("assigned", assigned);
}

void	showOutOfBounds()
{
	std::cout << YELLOW << "---exception test---" << RESET << std::endl;

	Array<int>	empty;
	Array<int>	numbers(3);

	try
	{
		empty[0] = 1;
	}
	catch (std::exception & e)
	{
		std::cout << RED << "empty[0]: " << e.what() << RESET << std::endl;
	}
	try
	{
		numbers[3] = 1;
	}
	catch (std::exception & e)
	{
		std::cout << RED << "numbers[3]: " << e.what() << RESET << std::endl;
	}
}

int	main(void)
{
	showDefaultInit();
	showDeepCopy();
	showOutOfBounds();
}
