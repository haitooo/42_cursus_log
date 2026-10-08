#include "easyfind.hpp"
#include "TextFormatter.h"
#include <deque>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <vector>

static int	g_pass = 0;
static int	g_fail = 0;

static void	check(std::string const & label, bool ok)
{
	if (ok)
	{
		std::cout << GREEN << "  ✓ " << RESET << label << std::endl;
		g_pass++;
	}
	else
	{
		std::cout << RED << "  × " << RESET << label << std::endl;
		g_fail++;
	}
}

static void	section(std::string const & title)
{
	std::cout << std::endl << YELLOW << title << RESET << std::endl;
}

template <typename T>
static bool	throwsOnFind(T const & container, int value)
{
	try
	{
		easyfind(container, value);
	}
	catch (std::runtime_error &)
	{
		return (true);
	}

	return (false);
}

static void	testVector()
{
	section("easyfind: std::vector");

	std::vector<int>	numbers;

	numbers.push_back(10);
	numbers.push_back(20);
	numbers.push_back(30);

	check("finds the first element", easyfind(numbers, 10) == numbers.begin());
	check("finds the last element", *easyfind(numbers, 30) == 30);
	check("returns the right position", std::distance(numbers.begin(), easyfind(numbers, 20)) == 1);
	check("missing value throws", throwsOnFind(numbers, 42));
}

static void	testList()
{
	section("easyfind: std::list");

	std::list<int>	numbers;

	numbers.push_back(5);
	numbers.push_back(7);
	numbers.push_back(5);

	check("returns the first occurrence", easyfind(numbers, 5) == numbers.begin());
	check("finds a middle element", *easyfind(numbers, 7) == 7);
	check("missing value throws", throwsOnFind(numbers, 0));
}

static void	testDeque()
{
	section("easyfind: std::deque");

	std::deque<int>	numbers;

	numbers.push_back(-1);
	numbers.push_back(0);

	check("finds a negative value", *easyfind(numbers, -1) == -1);
	check("finds zero", *easyfind(numbers, 0) == 0);
	check("missing value throws", throwsOnFind(numbers, 1));
}

static void	testEmpty()
{
	section("easyfind: empty container");

	std::vector<int>	empty;

	check("empty container throws", throwsOnFind(empty, 0));
}

static void	testConstAndMutable()
{
	section("easyfind: const / non-const overloads");

	std::vector<int>		numbers(3, 1);
	std::vector<int> const	&view = numbers;

	*easyfind(numbers, 1) = 9;
	check("non-const overload allows modification", numbers[0] == 9);
	check("const overload returns const_iterator", easyfind(view, 1) == view.begin() + 1);
	check("exception is catchable as std::exception", throwsOnFind(view, 2));
}

static void	runAllTests()
{
	testVector();
	testList();
	testDeque();
	testEmpty();
	testConstAndMutable();
}

int	main(void)
{
	runAllTests();

	std::cout << std::endl;
	if (g_fail == 0)
		std::cout << GREEN << "all passed: " << g_pass << " / " << (g_pass + g_fail) << RESET << std::endl;
	else
		std::cout << RED << "failed: " << g_fail << " / " << (g_pass + g_fail) << RESET << std::endl;

	return (g_fail == 0 ? 0 : 1);
}
