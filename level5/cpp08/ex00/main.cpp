#include "easyfind.hpp"
#include "TextFormatter.h"
#include <iostream>
#include <list>
#include <vector>

template <typename T>
void	tryFind(T const & container, int value)
{
	std::cout << "find " << value << " : ";
	try
	{
		easyfind(container, value);
		std::cout << GREEN << "found" << RESET << std::endl;
	}
	catch (std::runtime_error & e)
	{
		std::cout << RED << e.what() << RESET << std::endl;
	}
}

int	main(void)
{
	std::vector<int>	vec;
	std::list<int>		lst;
	std::vector<int>	empty;

	for (int i = 1; i <= 5; i++)
	{
		vec.push_back(i);
		lst.push_back(i * 10);
	}

	std::cout << YELLOW << "---vector {1, 2, 3, 4, 5}---" << RESET << std::endl;
	tryFind(vec, 1);
	tryFind(vec, 5);
	tryFind(vec, 42);

	std::cout << YELLOW << "---list {10, 20, 30, 40, 50}---" << RESET << std::endl;
	tryFind(lst, 30);
	tryFind(lst, 42);

	std::cout << YELLOW << "---empty vector---" << RESET << std::endl;
	tryFind(empty, 0);

	return (0);
}
