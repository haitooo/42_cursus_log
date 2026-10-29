#include "RPN.hpp"
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

static bool	evaluatesTo(std::string const & expression, long expected)
{
	try
	{
		RPN	rpn(expression);

		return (rpn.calculate() == expected);
	}
	catch (std::exception &)
	{
		return (false);
	}
}

static bool	throwsError(std::string const & expression)
{
	try
	{
		RPN	rpn(expression);

		rpn.calculate();
	}
	catch (RPN::RunTimeErrorException &)
	{
		return (true);
	}

	return (false);
}

static void	testSubjectExamples()
{
	section("RPN: subject examples");

	check("\"8 9 * 9 - 9 - 9 - 4 - 1 +\" is 42", evaluatesTo("8 9 * 9 - 9 - 9 - 4 - 1 +", 42));
	check("\"7 7 * 7 -\" is 42", evaluatesTo("7 7 * 7 -", 42));
	check("\"1 2 * 2 / 2 * 2 4 - +\" is 0", evaluatesTo("1 2 * 2 / 2 * 2 4 - +", 0));
	check("\"(1 + 1)\" throws", throwsError("(1 + 1)"));
}

static void	testOperators()
{
	section("RPN: each operator");

	check("\"3 4 +\" is 7", evaluatesTo("3 4 +", 7));
	check("\"3 4 -\" is -1", evaluatesTo("3 4 -", -1));
	check("\"3 4 *\" is 12", evaluatesTo("3 4 *", 12));
	check("\"8 2 /\" is 4", evaluatesTo("8 2 /", 4));
	check("\"0 0 +\" is 0", evaluatesTo("0 0 +", 0));
}

static void	testOperandOrder()
{
	section("RPN: operand order (left is pushed first)");

	check("\"9 1 -\" is 8, not -8", evaluatesTo("9 1 -", 8));
	check("\"8 4 /\" is 2, not 0", evaluatesTo("8 4 /", 2));
	check("\"1 2 3 - -\" is 1 - (2 - 3) = 2", evaluatesTo("1 2 3 - -", 2));
	check("\"1 2 - 3 -\" is (1 - 2) - 3 = -4", evaluatesTo("1 2 - 3 -", -4));
}

static void	testIntegerDivision()
{
	section("RPN: integer division");

	check("\"7 2 /\" truncates to 3", evaluatesTo("7 2 /", 3));
	check("\"0 7 - 2 /\" truncates toward zero to -3", evaluatesTo("0 7 - 2 /", -3));
	check("\"0 5 /\" is 0", evaluatesTo("0 5 /", 0));
}

static void	testSingleNumber()
{
	section("RPN: a single number");

	check("\"0\" is 0", evaluatesTo("0", 0));
	check("\"9\" is 9", evaluatesTo("9", 9));
}

static void	testLargeResult()
{
	section("RPN: results beyond the int range");

	check("9^10 is 3486784401",
		evaluatesTo("9 9 * 9 * 9 * 9 * 9 * 9 * 9 * 9 * 9 *", 3486784401L));
	check("-(9^10) is -3486784401",
		evaluatesTo("0 9 9 * 9 * 9 * 9 * 9 * 9 * 9 * 9 * 9 * -", -3486784401L));
}

static void	testWhitespace()
{
	section("RPN: whitespace between tokens");

	check("multiple spaces are accepted", evaluatesTo("  1    2   + ", 3));
	check("tabs are accepted", evaluatesTo("1\t2\t+", 3));
	check("newlines are accepted", evaluatesTo("1\n2\n+", 3));
}

static void	testInvalidTokens()
{
	section("RPN: invalid tokens throw");

	check("multi-digit number \"12 3 +\"", throwsError("12 3 +"));
	check("negative literal \"-1 2 +\"", throwsError("-1 2 +"));
	check("decimal \"1.5 2 +\"", throwsError("1.5 2 +"));
	check("operator stuck to a number \"1 2 +3\"", throwsError("1 2 +3"));
	check("tokens without spaces \"12+\"", throwsError("12+"));
	check("unknown operator \"1 2 %\"", throwsError("1 2 %"));
	check("letter \"a 1 +\"", throwsError("a 1 +"));
}

static void	testInvalidExpressions()
{
	section("RPN: malformed expressions throw");

	check("empty string", throwsError(""));
	check("only spaces", throwsError("   "));
	check("operator only \"+\"", throwsError("+"));
	check("not enough operands \"1 +\"", throwsError("1 +"));
	check("operator before operands \"+ 1 2\"", throwsError("+ 1 2"));
	check("leftover operands \"1 2\"", throwsError("1 2"));
	check("leftover operands \"1 2 3 +\"", throwsError("1 2 3 +"));
}

static void	testDivisionByZero()
{
	section("RPN: division by zero throws");

	check("\"1 0 /\"", throwsError("1 0 /"));
	check("\"0 0 /\"", throwsError("0 0 /"));
	check("divisor computed to zero \"4 2 2 - /\"", throwsError("4 2 2 - /"));
}

static void	testExceptionType()
{
	section("RPN: exception type and message");

	bool	caught = false;
	try { RPN("1 +").calculate(); }
	catch (std::runtime_error &) { caught = true; }
	check("RunTimeErrorException is catchable as std::runtime_error", caught);

	caught = false;
	try { RPN("1 0 /").calculate(); }
	catch (std::exception &) { caught = true; }
	check("RunTimeErrorException is catchable as std::exception", caught);

	bool	startsWithError = false;
	try { RPN("(1 + 1)").calculate(); }
	catch (std::exception & e) { startsWithError = std::string(e.what()).find("Error") == 0; }
	check("what() starts with \"Error\"", startsWithError);
}

static void	testRepeatedCalculate()
{
	section("RPN: calculate() can be called again");

	RPN		rpn("3 4 +");
	long	first = rpn.calculate();
	long	second = rpn.calculate();

	check("second call gives the same result", first == 7 && second == 7);
}

static void	testCopy()
{
	section("RPN: copy constructor / copy assignment");

	RPN	original("2 3 *");
	RPN	copy(original);
	RPN	assigned("1 1 +");

	assigned = original;

	check("copy evaluates the same expression", copy.calculate() == 6);
	check("assignment replaces the expression", assigned.calculate() == 6);
	check("original is unchanged", original.calculate() == 6);
}

static void	runAllTests()
{
	testSubjectExamples();
	testOperators();
	testOperandOrder();
	testIntegerDivision();
	testSingleNumber();
	testLargeResult();
	testWhitespace();
	testInvalidTokens();
	testInvalidExpressions();
	testDivisionByZero();
	testExceptionType();
	testRepeatedCalculate();
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
