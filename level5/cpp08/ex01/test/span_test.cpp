#include "Span.hpp"
#include "TextFormatter.h"
#include <climits>
#include <iostream>
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

static void	testSubjectExample()
{
	section("Span: subject example");

	Span	sp(5);

	sp.addNumber(6);
	sp.addNumber(3);
	sp.addNumber(17);
	sp.addNumber(9);
	sp.addNumber(11);

	check("shortestSpan is 2", sp.shortestSpan() == 2);
	check("longestSpan is 14", sp.longestSpan() == 14);
}

static void	testAddNumberThrows()
{
	section("Span: addNumber rejects when full");

	Span	sp(2);
	Span	zero(0);
	bool	thrown;

	sp.addNumber(1);
	sp.addNumber(2);

	thrown = false;
	try { sp.addNumber(3); }
	catch (Span::FullException &) { thrown = true; }
	check("third number on Span(2) throws FullException", thrown);
	check("size is unchanged after the throw", sp.size() == 2);

	thrown = false;
	try { zero.addNumber(1); }
	catch (Span::FullException &) { thrown = true; }
	check("Span(0) accepts nothing", thrown);

	thrown = false;
	try { sp.addNumber(4); }
	catch (std::length_error &) { thrown = true; }
	check("FullException is catchable as std::length_error", thrown);
}

static void	testNoSpanThrows()
{
	section("Span: shortestSpan / longestSpan need two numbers");

	Span	sp(3);
	bool	thrown;

	thrown = false;
	try { sp.shortestSpan(); }
	catch (Span::NoSpanException &) { thrown = true; }
	check("shortestSpan on empty throws", thrown);

	sp.addNumber(1);

	thrown = false;
	try { sp.longestSpan(); }
	catch (Span::NoSpanException &) { thrown = true; }
	check("longestSpan with one number throws", thrown);

	thrown = false;
	try { sp.shortestSpan(); }
	catch (std::logic_error &) { thrown = true; }
	check("NoSpanException is catchable as std::logic_error", thrown);

	thrown = false;
	try { sp.shortestSpan(); }
	catch (std::exception &) { thrown = true; }
	check("exception is catchable as std::exception", thrown);

	bool	messageKept = false;
	try { sp.shortestSpan(); }
	catch (std::exception & e) { messageKept = std::string(e.what()).find("Span:") == 0; }
	check("what() returns the message passed to the base", messageKept);
}

static void	testDuplicatesAndNegatives()
{
	section("Span: duplicates and negative numbers");

	Span	dup(3);
	Span	neg(3);

	dup.addNumber(5);
	dup.addNumber(5);
	dup.addNumber(10);
	check("duplicates give shortestSpan 0", dup.shortestSpan() == 0);

	neg.addNumber(-10);
	neg.addNumber(-3);
	neg.addNumber(4);
	check("shortestSpan with negatives", neg.shortestSpan() == 7);
	check("longestSpan with negatives", neg.longestSpan() == 14);
}

static void	testExtremes()
{
	section("Span: INT_MIN / INT_MAX do not overflow");

	Span	sp(3);

	sp.addNumber(INT_MIN);
	sp.addNumber(0);
	sp.addNumber(INT_MAX);

	check("shortestSpan is INT_MAX", sp.shortestSpan() == static_cast<unsigned int>(INT_MAX));
	check("longestSpan is UINT_MAX", sp.longestSpan() == UINT_MAX);
}

static void	testRangeInsert()
{
	section("Span: addNumber with a range of iterators");

	std::list<int>	source;
	Span			sp(4);
	bool			thrown;

	source.push_back(1);
	source.push_back(10);
	source.push_back(4);
	sp.addNumber(source.begin(), source.end());
	check("all numbers are added", sp.size() == 3);
	check("shortestSpan after range insert", sp.shortestSpan() == 3);

	thrown = false;
	try { sp.addNumber(source.begin(), source.end()); }
	catch (Span::FullException &) { thrown = true; }
	check("range larger than the room throws", thrown);
	check("nothing is added on failure", sp.size() == 3);

	sp.addNumber(source.begin(), source.begin());
	check("empty range adds nothing", sp.size() == 3);
}

static void	testBigSpan()
{
	section("Span: 100000 numbers");

	std::vector<int>	source;
	Span				sp(100000);

	for (int i = 0; i < 100000; i++)
		source.push_back(i * 3);
	sp.addNumber(source.begin(), source.end());

	check("size is 100000", sp.size() == 100000);
	check("shortestSpan is 3", sp.shortestSpan() == 3);
	check("longestSpan is 299997", sp.longestSpan() == 299997);
}

static void	testCopy()
{
	section("Span: copy constructor / copy assignment");

	Span	original(3);

	original.addNumber(1);
	original.addNumber(5);

	Span	copy(original);
	Span	assigned(10);

	assigned = original;
	copy.addNumber(2);

	check("copy keeps the capacity", copy.getCapacity() == 3);
	check("copy does not share state with original", original.size() == 2 && copy.size() == 3);
	check("assignment copies the capacity", assigned.getCapacity() == 3);
	check("assignment copies the numbers", assigned.longestSpan() == 4);
}

static void	runAllTests()
{
	testSubjectExample();
	testAddNumberThrows();
	testNoSpanThrows();
	testDuplicatesAndNegatives();
	testExtremes();
	testRangeInsert();
	testBigSpan();
	testCopy();
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
