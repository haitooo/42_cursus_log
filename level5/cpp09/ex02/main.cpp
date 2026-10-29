#include "PmergeMe.hpp"
#include <ctime>
#include <iomanip>
#include <iostream>

static double	elapsedUs(std::clock_t start, std::clock_t end)
{
	return (static_cast<double>(end - start) * 1000000.0 / CLOCKS_PER_SEC);
}

int	main(int argc, char **argv)
{
	if (argc < 2)
	{
		std::cerr << "Error" << std::endl;
		return (1);
	}

	PmergeMe		sorter;
	std::clock_t	start;
	double			vectorUs;
	double			dequeUs;

	try
	{
		// 計測には引数の読み込み（コンテナへの格納）と並べ替えの両方を含める
		start = std::clock();
		sorter.sortVector(argc - 1, argv + 1);
		vectorUs = elapsedUs(start, std::clock());

		start = std::clock();
		sorter.sortDeque(argc - 1, argv + 1);
		dequeUs = elapsedUs(start, std::clock());
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return (1);
	}

	const std::vector<unsigned int>&	sorted = sorter.getVector();

	std::cout << "Before:";
	for (int i = 1; i < argc; ++i)
		std::cout << " " << argv[i];
	std::cout << std::endl;

	std::cout << "After: ";
	for (std::size_t i = 0; i < sorted.size(); ++i)
		std::cout << " " << sorted[i];
	std::cout << std::endl;

	std::cout << std::fixed << std::setprecision(5);
	std::cout << "Time to process a range of " << sorted.size()
		<< " elements with std::vector : " << vectorUs << " us"
		<< " (" << sorter.getVectorComparisons() << " comparisons)" << std::endl;
	std::cout << "Time to process a range of " << sorter.getDeque().size()
		<< " elements with std::deque  : " << dequeUs << " us"
		<< " (" << sorter.getDequeComparisons() << " comparisons)" << std::endl;

	return (0);
}
