#include "Span.hpp"
#include "TextFormatter.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

#define BIG_SIZE 20000

void	showSubject()
{
	std::cout << YELLOW << "---subject test---" << RESET << std::endl;

	Span	sp = Span(5);

	sp.addNumber(6);
	sp.addNumber(3);
	sp.addNumber(17);
	sp.addNumber(9);
	sp.addNumber(11);

	std::cout << sp.shortestSpan() << std::endl;
	std::cout << sp.longestSpan() << std::endl;
}

void	showBigSpan()
{
	std::cout << YELLOW << "---" << BIG_SIZE << " numbers test---" << RESET << std::endl;

	std::vector<int>	numbers;
	Span				sp(BIG_SIZE);

	std::srand(std::time(NULL));
	for (int i = 0; i < BIG_SIZE; i++)
		numbers.push_back(std::rand());
	sp.addNumber(numbers.begin(), numbers.end());

	std::cout << "shortest : " << sp.shortestSpan() << std::endl;
	std::cout << "longest  : " << sp.longestSpan() << std::endl;
}

void	showExceptions()
{
	std::cout << YELLOW << "---exception test---" << RESET << std::endl;

	Span	sp(1);

	sp.addNumber(42);
	try
	{
		sp.addNumber(43);
	}
	catch (std::length_error & e)
	{
		std::cout << RED << e.what() << RESET << std::endl;
	}
	try
	{
		sp.shortestSpan();
	}
	catch (std::logic_error & e)
	{
		std::cout << RED << e.what() << RESET << std::endl;
	}
}

int	main(void)
{
	showSubject();
	showBigSpan();
	showExceptions();

	return (0);
}
