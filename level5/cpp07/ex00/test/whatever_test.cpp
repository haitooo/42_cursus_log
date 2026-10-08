#include "whatever.hpp"
#include "TextFormatter.h"
#include <iostream>
#include <string>

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

static void	testSwap()
{
	section("swap");

	int			a = 2;
	int			b = 3;
	std::string	c = "chaine1";
	std::string	d = "chaine2";
	double		e = 4.2;
	double		f = -1.5;

	::swap(a, b);
	check("int values are swapped", a == 3 && b == 2);

	::swap(c, d);
	check("std::string values are swapped", c == "chaine2" && d == "chaine1");

	::swap(e, f);
	check("double values are swapped", e == -1.5 && f == 4.2);

	::swap(a, a);
	check("swapping with itself keeps the value", a == 3);
}

static void	testMin()
{
	section("min");

	int			a = 2;
	int			b = 3;
	std::string	c = "chaine1";
	std::string	d = "chaine2";
	char		e = 'z';
	char		f = 'a';

	check("returns the smaller int", ::min(a, b) == 2);
	check("order of arguments does not matter", ::min(b, a) == 2);
	check("returns the smaller std::string", ::min(c, d) == "chaine1");
	check("returns the smaller char", ::min(e, f) == 'a');
}

static void	testMax()
{
	section("max");

	int			a = 2;
	int			b = 3;
	std::string	c = "chaine1";
	std::string	d = "chaine2";
	char		e = 'z';
	char		f = 'a';

	check("returns the greater int", ::max(a, b) == 3);
	check("order of arguments does not matter", ::max(b, a) == 3);
	check("returns the greater std::string", ::max(c, d) == "chaine2");
	check("returns the greater char", ::max(e, f) == 'z');
}

static void	testEqualReturnsSecond()
{
	section("equal values return the second argument");

	int			x = 42;
	int			y = 42;
	std::string	s = "same";
	std::string	t = "same";

	check("min(x, y) returns y", &::min(x, y) == &y);
	check("max(x, y) returns y", &::max(x, y) == &y);
	check("min(s, t) returns t", &::min(s, t) == &t);
	check("max(s, t) returns t", &::max(s, t) == &t);
}

static void	runAllTests()
{
	testSwap();
	testMin();
	testMax();
	testEqualReturnsSecond();
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
