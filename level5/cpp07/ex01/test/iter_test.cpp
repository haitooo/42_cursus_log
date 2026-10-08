#include "iter.hpp"
#include "TextFormatter.h"
#include <iostream>
#include <string>

static int	g_pass = 0;
static int	g_fail = 0;
static int	g_calls = 0;
static int	g_sum = 0;

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
void	doubleValue(T & value)
{
	value = value + value;
}

static void	countCall(int const & value)
{
	g_calls++;
	g_sum += value;
}

template <typename T>
void	countAny(T const & value)
{
	(void)value;
	g_calls++;
}

static void	resetCounters()
{
	g_calls = 0;
	g_sum = 0;
}

static void	testNonConstElements()
{
	section("iter: non-const elements with non-const reference function");

	int			ints[] = {1, 2, 3};
	std::string	strs[] = {"a", "bc"};

	::iter(ints, 3, doubleValue<int>);
	check("int elements are modified", ints[0] == 2 && ints[1] == 4 && ints[2] == 6);

	::iter(strs, 2, doubleValue<std::string>);
	check("std::string elements are modified", strs[0] == "aa" && strs[1] == "bcbc");
}

static void	testConstReferenceFunction()
{
	section("iter: const reference function");

	int			ints[] = {1, 2, 3};
	int const	constInts[] = {10, 20, 30};

	resetCounters();
	::iter(ints, 3, countCall);
	check("called on every element of a non-const array", g_calls == 3 && g_sum == 6);

	resetCounters();
	::iter(constInts, 3, countCall);
	check("called on every element of a const array", g_calls == 3 && g_sum == 60);

	resetCounters();
	::iter(constInts, 3, countAny<int>);
	check("accepts an instantiated function template", g_calls == 3);
}

static void	testLength()
{
	section("iter: length handling");

	int	ints[] = {1, 2, 3, 4, 5};

	resetCounters();
	::iter(ints, 2, countCall);
	check("only the first `length` elements are visited", g_calls == 2 && g_sum == 3);

	resetCounters();
	::iter(ints, 0, countCall);
	check("length 0 calls nothing", g_calls == 0);

	resetCounters();
	::iter(static_cast<int *>(NULL), 5, countCall);
	check("NULL array calls nothing", g_calls == 0);
}

static void	runAllTests()
{
	testNonConstElements();
	testConstReferenceFunction();
	testLength();
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
