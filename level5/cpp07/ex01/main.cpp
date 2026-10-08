#include "iter.hpp"
#include "TextFormatter.h"
#include <iostream>
#include <string>

template <typename T>
void	print(T const & value)
{
	std::cout << "[" << value << "] ";
}

template <typename T>
void	increment(T & value)
{
	value++;
}

void	toUpper(std::string & str)
{
	for (std::size_t i = 0; i < str.size(); i++)
	{
		if (str[i] >= 'a' && str[i] <= 'z')
			str[i] = str[i] - 'a' + 'A';
	}
}

int	main(void)
{
	int				ints[] = {0, 1, 2, 3, 4};
	std::string		strs[] = {"hello", "templates", "world"};
	double			doubles[] = {1.5, 2.25, -3.0};

	std::cout << YELLOW << "---int array---" << RESET << std::endl;
	std::cout << "original    : ";
	::iter(ints, 5, print<int>);
	std::cout << std::endl;
	::iter(ints, 5, increment<int>);
	std::cout << "incremented : ";
	::iter(ints, 5, print<int>);
	std::cout << std::endl;

	std::cout << YELLOW << "---string array---" << RESET << std::endl;
	std::cout << "original    : ";
	::iter(strs, 3, print<std::string>);
	std::cout << std::endl;
	::iter(strs, 3, toUpper);
	std::cout << "uppercased  : ";
	::iter(strs, 3, print<std::string>);
	std::cout << std::endl;

	std::cout << YELLOW << "---double array---" << RESET << std::endl;
	std::cout << "original    : ";
	::iter(doubles, 3, print<double>);
	std::cout << std::endl;
	::iter(doubles, 3, increment<double>);
	std::cout << "incremented : ";
	::iter(doubles, 3, print<double>);
	std::cout << std::endl;

	return (0);
}
