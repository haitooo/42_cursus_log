#include "MutantStack.hpp"
#include "TextFormatter.h"
#include <algorithm>
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

static void	testStackInterface()
{
	section("MutantStack: std::stack member functions");

	MutantStack<int>	mstack;

	check("new stack is empty", mstack.empty() && mstack.size() == 0);

	mstack.push(5);
	mstack.push(17);
	check("top is the last pushed", mstack.top() == 17);
	check("size counts pushes", mstack.size() == 2);

	mstack.pop();
	check("pop removes the top", mstack.top() == 5 && mstack.size() == 1);
}

static void	testIteration()
{
	section("MutantStack: iterators");

	MutantStack<int>	mstack;
	std::vector<int>	visited;

	mstack.push(1);
	mstack.push(2);
	mstack.push(3);
	for (MutantStack<int>::iterator it = mstack.begin(); it != mstack.end(); ++it)
		visited.push_back(*it);
	check("begin to end goes bottom to top", visited.size() == 3 && visited[0] == 1 && visited[2] == 3);

	MutantStack<int>::iterator	it = mstack.begin();

	++it;
	--it;
	check("++ then -- returns to begin", it == mstack.begin());

	*mstack.begin() = 42;
	check("writing through iterator changes the element", *mstack.begin() == 42);

	check("works with <algorithm>", std::find(mstack.begin(), mstack.end(), 3) != mstack.end());

	MutantStack<int>	empty;

	check("empty stack has begin == end", empty.begin() == empty.end());
}

static void	testReverseAndConst()
{
	section("MutantStack: reverse / const iterators");

	MutantStack<int>	mstack;

	mstack.push(1);
	mstack.push(2);
	mstack.push(3);

	MutantStack<int> const &	view = mstack;

	check("rbegin points to the top", *mstack.rbegin() == mstack.top());
	check("const begin reads the bottom", *view.begin() == 1);
	check("const rbegin reads the top", *view.rbegin() == 3);
	check("distance matches size", static_cast<unsigned long>(std::distance(view.begin(), view.end())) == view.size());
}

static void	testMatchesList()
{
	section("MutantStack: same result as std::list");

	MutantStack<int>	mstack;
	std::list<int>		lst;
	int const			values[] = {5, 17, 3, 5, 737, 0};

	for (unsigned int i = 0; i < sizeof(values) / sizeof(values[0]); i++)
	{
		mstack.push(values[i]);
		lst.push_back(values[i]);
	}
	check("same elements in the same order", std::equal(mstack.begin(), mstack.end(), lst.begin()));
}

static void	testCopy()
{
	section("MutantStack: copy constructor / copy assignment");

	MutantStack<int>	original;

	original.push(1);
	original.push(2);

	MutantStack<int>	copy(original);
	MutantStack<int>	assigned;

	assigned.push(99);
	assigned = original;
	copy.push(3);

	check("copy does not share state with original", original.size() == 2 && copy.size() == 3);
	check("assignment copies the elements", std::equal(assigned.begin(), assigned.end(), original.begin()));

	std::stack<int>	s(original);

	check("converts to std::stack", s.top() == 2 && s.size() == 2);
}

static void	testOtherContainer()
{
	section("MutantStack: other underlying containers");

	MutantStack<int, std::vector<int> >	onVector;
	MutantStack<int, std::list<int> >	onList;

	onVector.push(1);
	onVector.push(2);
	onList.push(1);
	onList.push(2);

	check("std::vector based stack iterates", *onVector.begin() == 1 && onVector.top() == 2);
	check("std::list based stack iterates", *onList.begin() == 1 && onList.top() == 2);
}

static void	runAllTests()
{
	testStackInterface();
	testIteration();
	testReverseAndConst();
	testMatchesList();
	testCopy();
	testOtherContainer();
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
