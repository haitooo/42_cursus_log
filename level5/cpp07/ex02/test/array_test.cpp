#include "Array.hpp"
#include "TextFormatter.h"
#include <iostream>
#include <stdexcept>
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

class	ThrowOnAssign {
	public:
		static int	assignCount;
		static int	throwAt;
		int			value;

		ThrowOnAssign() : value(0) {}
		ThrowOnAssign(const ThrowOnAssign& other) : value(other.value) {}
		ThrowOnAssign&	operator=(const ThrowOnAssign& other)
		{
			if (++assignCount == throwAt)
				throw std::runtime_error("assignment failed");
			value = other.value;

			return (*this);
		}
		~ThrowOnAssign() {}
};

int	ThrowOnAssign::assignCount = 0;
int	ThrowOnAssign::throwAt = 0;

static void	testDefaultConstructor()
{
	section("Array: default constructor");

	Array<int>	array;
	bool		thrown;

	check("size is 0", array.size() == 0);

	thrown = false;
	try { array[0] = 1; }
	catch (std::exception &) { thrown = true; }
	check("accessing index 0 throws", thrown);
}

static void	testSizeConstructor()
{
	section("Array: constructor with n");

	Array<int>			ints(5);
	Array<std::string>	strs(3);
	Array<int>			zero(0);
	bool				allZero = true;
	bool				allEmpty = true;

	for (unsigned int i = 0; i < ints.size(); i++)
		allZero = allZero && ints[i] == 0;
	for (unsigned int i = 0; i < strs.size(); i++)
		allEmpty = allEmpty && strs[i].empty();

	check("size is n", ints.size() == 5 && strs.size() == 3);
	check("int elements are initialized to 0", allZero);
	check("std::string elements are empty", allEmpty);
	check("n = 0 creates an empty array", zero.size() == 0);
}

static void	testCopyConstructor()
{
	section("Array: copy constructor");

	Array<int>	original(3);

	original[0] = 1;
	original[1] = 2;
	original[2] = 3;

	Array<int>	copy(original);

	check("size is copied", copy.size() == 3);
	check("elements are copied", copy[0] == 1 && copy[1] == 2 && copy[2] == 3);

	copy[0] = 100;
	check("modifying the copy does not affect the original", original[0] == 1);

	original[1] = 200;
	check("modifying the original does not affect the copy", copy[1] == 2);

	Array<int>	empty;
	Array<int>	emptyCopy(empty);

	check("copying an empty array gives an empty array", emptyCopy.size() == 0);
}

static void	testAssignment()
{
	section("Array: copy assignment");

	Array<std::string>	source(2);
	Array<std::string>	target(5);
	Array<std::string>&	alias = source;

	source[0] = "foo";
	source[1] = "bar";

	target = source;
	check("size follows the source", target.size() == 2);
	check("elements are assigned", target[0] == "foo" && target[1] == "bar");

	target[0] = "changed";
	check("modifying the target does not affect the source", source[0] == "foo");

	source[1] = "changed";
	check("modifying the source does not affect the target", target[1] == "bar");

	source = alias;
	check("self assignment keeps the value", source.size() == 2 && source[0] == "foo");

	Array<std::string>	empty;

	target = empty;
	check("assigning an empty array empties the target", target.size() == 0);
}

static void	testSubscript()
{
	section("Array: operator[]");

	Array<int>	array(3);
	bool		thrown;

	array[2] = 42;
	check("read back the written value", array[2] == 42);

	thrown = false;
	try { array[3] = 0; }
	catch (Array<int>::OutOfBoundsException &) { thrown = true; }
	check("index == size throws OutOfBoundsException", thrown);

	thrown = false;
	try { array[-1] = 0; }
	catch (std::exception &) { thrown = true; }
	check("negative index (wraps to large unsigned) throws", thrown);

	Array<int> const	constArray(array);

	check("const array can be read", constArray[2] == 42);

	thrown = false;
	try { (void)constArray[3]; }
	catch (std::exception &) { thrown = true; }
	check("const operator[] also throws out of bounds", thrown);
}

static void	testSize()
{
	section("Array: size");

	Array<double>			array(7);
	Array<double> const &	constRef = array;

	check("size() works on a const reference", constRef.size() == 7);
}

static void	testExceptionSafety()
{
	section("Array: exception safety when T's assignment throws");

	Array<ThrowOnAssign>	source(3);
	Array<ThrowOnAssign>	target(1);
	bool					thrown;

	ThrowOnAssign::throwAt = 0;
	source[0].value = 1;
	source[1].value = 2;
	source[2].value = 3;
	target[0].value = 42;

	ThrowOnAssign::assignCount = 0;
	ThrowOnAssign::throwAt = 2;
	thrown = false;
	try
	{
		Array<ThrowOnAssign>	copy(source);
		(void)copy;
	}
	catch (std::runtime_error &) { thrown = true; }
	check("copy constructor propagates the exception", thrown);

	ThrowOnAssign::assignCount = 0;
	thrown = false;
	try { target = source; }
	catch (std::runtime_error &) { thrown = true; }
	check("operator= propagates the exception", thrown);
	check("operator= leaves the target unchanged", target.size() == 1 && target[0].value == 42);
	ThrowOnAssign::throwAt = 0;
}

static void	runAllTests()
{
	testDefaultConstructor();
	testSizeConstructor();
	testCopyConstructor();
	testAssignment();
	testSubscript();
	testSize();
	testExceptionSafety();
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
