#include "PmergeMe.hpp"
#include "TextFormatter.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <sstream>
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

static std::string	toString(unsigned int n)
{
	std::ostringstream	oss;

	oss << n;
	return (oss.str());
}

static bool	sortsCorrectly(std::vector<std::string> args)
{
	std::vector<char *>			argv;
	std::vector<unsigned int>	expected;
	PmergeMe					sorter;

	for (std::size_t i = 0; i < args.size(); ++i)
	{
		argv.push_back(&args[i][0]);
		expected.push_back(static_cast<unsigned int>(std::strtoul(args[i].c_str(), NULL, 10)));
	}
	std::sort(expected.begin(), expected.end());

	sorter.sortVector(static_cast<int>(argv.size()), &argv[0]);
	sorter.sortDeque(static_cast<int>(argv.size()), &argv[0]);

	const std::vector<unsigned int>&	vec = sorter.getVector();
	const std::deque<unsigned int>&		deq = sorter.getDeque();

	return (vec == expected
		&& deq.size() == expected.size()
		&& std::equal(deq.begin(), deq.end(), expected.begin()));
}

static bool	throwsError(std::string arg)
{
	char	*argv[] = {&arg[0]};
	bool	vectorThrown = false;
	bool	dequeThrown = false;
	PmergeMe	sorter;

	try { sorter.sortVector(1, argv); }
	catch (PmergeMe::InvalidInputException &) { vectorThrown = true; }
	try { sorter.sortDeque(1, argv); }
	catch (PmergeMe::InvalidInputException &) { dequeThrown = true; }

	return (vectorThrown && dequeThrown);
}

static std::vector<std::string>	randomArgs(std::size_t n, unsigned int max)
{
	std::vector<std::string>	args;

	for (std::size_t i = 0; i < n; ++i)
		args.push_back(toString(static_cast<unsigned int>(std::rand()) % max + 1));
	return (args);
}

static void	testSubjectExample()
{
	section("PmergeMe: subject example");

	std::vector<std::string>	args;

	args.push_back("3");
	args.push_back("5");
	args.push_back("9");
	args.push_back("7");
	args.push_back("4");
	check("3 5 9 7 4 -> 3 4 5 7 9", sortsCorrectly(args));
}

static void	testSmallSizes()
{
	section("PmergeMe: every size from 1 to 64 (random values)");

	bool	ok = true;

	for (std::size_t n = 1; n <= 64; ++n)
	{
		for (int trial = 0; trial < 20; ++trial)
		{
			if (!sortsCorrectly(randomArgs(n, 1000)))
				ok = false;
		}
	}
	check("1..64 elements x 20 trials match std::sort", ok);
}

static void	testOrderedInputs()
{
	section("PmergeMe: already sorted / reversed input");

	std::vector<std::string>	ascending;
	std::vector<std::string>	descending;

	for (unsigned int i = 1; i <= 100; ++i)
	{
		ascending.push_back(toString(i));
		descending.push_back(toString(101 - i));
	}
	check("ascending 1..100", sortsCorrectly(ascending));
	check("descending 100..1", sortsCorrectly(descending));
}

static void	testDuplicates()
{
	section("PmergeMe: duplicates are kept");

	std::vector<std::string>	same(17, "42");

	check("17 copies of 42", sortsCorrectly(same));
	check("200 values from 1..5", sortsCorrectly(randomArgs(200, 5)));
}

static void	testBoundaries()
{
	section("PmergeMe: boundary values");

	std::vector<std::string>	args;

	args.push_back("2147483647");
	args.push_back("0");
	args.push_back("1");
	check("0, 1 and INT_MAX", sortsCorrectly(args));
}

static void	testLargeInput()
{
	section("PmergeMe: 3000 and 10000 elements");

	check("3000 random values from 1..100000", sortsCorrectly(randomArgs(3000, 100000)));
	check("10000 random values from 1..100000", sortsCorrectly(randomArgs(10000, 100000)));
}

static void	testSpaceSeparatedArgument()
{
	section("PmergeMe: numbers in one argument separated by spaces");

	std::string	arg = "5 3  1\t4";
	char		*argv[] = {&arg[0]};
	PmergeMe	sorter;

	sorter.sortVector(1, argv);
	sorter.sortDeque(1, argv);

	const std::vector<unsigned int>&	vec = sorter.getVector();

	check("\"5 3  1\\t4\" becomes 1 3 4 5",
		vec.size() == 4 && vec[0] == 1 && vec[1] == 3 && vec[2] == 4 && vec[3] == 5);
	check("deque has the same result", std::equal(vec.begin(), vec.end(), sorter.getDeque().begin()));
}

static void	testInvalidInput()
{
	section("PmergeMe: invalid input throws");

	check("negative \"-1\"", throwsError("-1"));
	check("explicit sign \"+1\"", throwsError("+1"));
	check("letters \"abc\"", throwsError("abc"));
	check("number followed by letters \"12a\"", throwsError("12a"));
	check("decimal \"1.5\"", throwsError("1.5"));
	check("over INT_MAX \"2147483648\"", throwsError("2147483648"));
	check("over unsigned long \"99999999999999999999\"", throwsError("99999999999999999999"));
	check("empty string", throwsError(""));
	check("only spaces", throwsError("   "));

	bool		messageIsError = false;
	PmergeMe	sorter;
	try { sorter.sortVector(0, NULL); }
	catch (std::exception & e) { messageIsError = std::string(e.what()) == "Error"; }
	check("no numbers throws and what() is \"Error\"", messageIsError);
}

// Knuth TAOCP Vol.3 5.3.1 の F(n) = Σ ceil(log2(3k/4))：Ford-Johnson の最悪比較回数
static std::size_t	fordJohnsonBound(std::size_t n)
{
	std::size_t	sum = 0;

	for (std::size_t k = 1; k <= n; ++k)
	{
		std::size_t	c = 0;

		while ((static_cast<std::size_t>(1) << c) * 4 < 3 * k)
			++c;
		sum += c;
	}
	return (sum);
}

static void	testComparisonCount()
{
	section("PmergeMe: comparisons never exceed F(n) for 1..21 elements");

	bool	vectorOk = true;
	bool	dequeOk = true;

	for (std::size_t n = 1; n <= 21; ++n)
	{
		for (int trial = 0; trial < 2000; ++trial)
		{
			std::vector<std::string>	args = randomArgs(n, 1000000);
			std::vector<char *>			argv;
			PmergeMe					sorter;

			for (std::size_t i = 0; i < args.size(); ++i)
				argv.push_back(&args[i][0]);
			sorter.sortVector(static_cast<int>(argv.size()), &argv[0]);
			sorter.sortDeque(static_cast<int>(argv.size()), &argv[0]);
			if (sorter.getVectorComparisons() > fordJohnsonBound(n))
				vectorOk = false;
			if (sorter.getDequeComparisons() > fordJohnsonBound(n))
				dequeOk = false;
		}
	}
	check("std::vector: comparisons <= F(n)", vectorOk);
	check("std::deque: comparisons <= F(n)", dequeOk);
	check("F(21) is 66", fordJohnsonBound(21) == 66);
}

static void	testCopy()
{
	section("PmergeMe: copy constructor / copy assignment");

	std::string	arg = "3 1 2";
	char		*argv[] = {&arg[0]};
	PmergeMe	original;

	original.sortVector(1, argv);
	original.sortDeque(1, argv);

	PmergeMe	copy(original);
	PmergeMe	assigned;

	assigned = original;

	check("copy has the sorted vector", copy.getVector() == original.getVector());
	check("copy has the sorted deque", copy.getDeque() == original.getDeque());
	check("assignment copies both containers",
		assigned.getVector() == original.getVector() && assigned.getDeque() == original.getDeque());
}

static void	runAllTests()
{
	testSubjectExample();
	testSmallSizes();
	testOrderedInputs();
	testDuplicates();
	testBoundaries();
	testLargeInput();
	testSpaceSeparatedArgument();
	testInvalidInput();
	testComparisonCount();
	testCopy();
}

int	main(void)
{
	std::srand(42);
	runAllTests();

	std::cout << std::endl;
	if (g_fail == 0)
		std::cout << GREEN << "all passed: " << g_pass << " / " << (g_pass + g_fail) << RESET << std::endl;
	else
		std::cout << RED << "failed: " << g_fail << " / " << (g_pass + g_fail) << RESET << std::endl;

	return (g_fail == 0 ? 0 : 1);
}
