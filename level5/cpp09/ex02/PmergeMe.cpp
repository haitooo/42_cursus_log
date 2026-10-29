#include "PmergeMe.hpp"
#include <algorithm>
#include <cctype>
#include <climits>
#include <cstdlib>
#include <sstream>

struct	VectorIndexLess {
	const std::vector<unsigned int>*	values;
	std::size_t*					comparisons;

	bool	operator()(std::size_t a, std::size_t b) const
	{
		++*comparisons;
		return ((*values)[a] < (*values)[b]);
	}
};

static std::vector<std::size_t>	insertionOrderVector(std::size_t pendCount)
{
	std::vector<std::size_t>	order;
	std::size_t					prev = 1;
	std::size_t					cur = 3;

	while (prev < pendCount)
	{
		for (std::size_t i = std::min(cur, pendCount); i > prev; --i)
			order.push_back(i);

		std::size_t	next = cur + 2 * prev;

		prev = cur;
		cur = next;
	}

	return (order);
}

static std::vector<std::size_t>	fordJohnsonVector(const std::vector<unsigned int>& values,
	const std::vector<std::size_t>& elems, std::size_t& comparisons)
{
	if (elems.size() <= 1)
		return (elems);

	VectorIndexLess				less = {&values, &comparisons};
	std::vector<std::size_t>	winners;
	std::vector<std::size_t>	loserOf(values.size());

	for (std::size_t i = 0; i + 1 < elems.size(); i += 2)
	{
		std::size_t	a = elems[i];
		std::size_t	b = elems[i + 1];

		if (less(a, b))
			std::swap(a, b);
		winners.push_back(a);
		loserOf[a] = b;
	}

	std::vector<std::size_t>	chain = fordJohnsonVector(values, winners, comparisons);
	std::vector<std::size_t>	sortedWinners = chain;
	std::vector<std::size_t>	pend;

	for (std::size_t i = 0; i < sortedWinners.size(); ++i)
		pend.push_back(loserOf[sortedWinners[i]]);
	if (elems.size() % 2 == 1)
		pend.push_back(elems.back());

	chain.insert(chain.begin(), pend[0]);

	std::vector<std::size_t>	order = insertionOrderVector(pend.size());

	for (std::size_t i = 0; i < order.size(); ++i)
	{
		std::size_t									k = order[i] - 1;
		std::vector<std::size_t>::iterator			bound = chain.end();

		if (k < sortedWinners.size())
			bound = std::find(chain.begin(), chain.end(), sortedWinners[k]);
		chain.insert(std::lower_bound(chain.begin(), bound, pend[k], less), pend[k]);
	}

	return (chain);
}

struct	DequeIndexLess {
	const std::deque<unsigned int>*	values;
	std::size_t*					comparisons;

	bool	operator()(std::size_t a, std::size_t b) const
	{
		++*comparisons;
		return ((*values)[a] < (*values)[b]);
	}
};

static std::deque<std::size_t>	insertionOrderDeque(std::size_t pendCount)
{
	std::deque<std::size_t>	order;
	std::size_t				prev = 1;
	std::size_t				cur = 3;

	while (prev < pendCount)
	{
		for (std::size_t i = std::min(cur, pendCount); i > prev; --i)
			order.push_back(i);

		std::size_t	next = cur + 2 * prev;

		prev = cur;
		cur = next;
	}

	return (order);
}

static std::deque<std::size_t>	fordJohnsonDeque(const std::deque<unsigned int>& values,
	const std::deque<std::size_t>& elems, std::size_t& comparisons)
{
	if (elems.size() <= 1)
		return (elems);

	DequeIndexLess			less = {&values, &comparisons};
	std::deque<std::size_t>	winners;
	std::deque<std::size_t>	loserOf(values.size());

	for (std::size_t i = 0; i + 1 < elems.size(); i += 2)
	{
		std::size_t	a = elems[i];
		std::size_t	b = elems[i + 1];

		if (less(a, b))
			std::swap(a, b);
		winners.push_back(a);
		loserOf[a] = b;
	}

	std::deque<std::size_t>	chain = fordJohnsonDeque(values, winners, comparisons);
	std::deque<std::size_t>	sortedWinners = chain;
	std::deque<std::size_t>	pend;

	for (std::size_t i = 0; i < sortedWinners.size(); ++i)
		pend.push_back(loserOf[sortedWinners[i]]);
	if (elems.size() % 2 == 1)
		pend.push_back(elems.back());

	chain.push_front(pend[0]);

	std::deque<std::size_t>	order = insertionOrderDeque(pend.size());

	for (std::size_t i = 0; i < order.size(); ++i)
	{
		std::size_t							k = order[i] - 1;
		std::deque<std::size_t>::iterator	bound = chain.end();

		if (k < sortedWinners.size())
			bound = std::find(chain.begin(), chain.end(), sortedWinners[k]);
		chain.insert(std::lower_bound(chain.begin(), bound, pend[k], less), pend[k]);
	}

	return (chain);
}

PmergeMe::PmergeMe() : vec(), deq(), vecComparisons(0), deqComparisons(0)
{
}

PmergeMe::PmergeMe(const PmergeMe& other)
	: vec(other.vec), deq(other.deq), vecComparisons(other.vecComparisons), deqComparisons(other.deqComparisons)
{
}

PmergeMe&	PmergeMe::operator=(const PmergeMe& other)
{
	if (this != &other)
	{
		vec = other.vec;
		deq = other.deq;
		vecComparisons = other.vecComparisons;
		deqComparisons = other.deqComparisons;
	}

	return (*this);
}

PmergeMe::~PmergeMe()
{
}

unsigned int	PmergeMe::toPositiveInt(const std::string& token)
{
	if (token.empty() || token.size() > 10)
		throw InvalidInputException();
	for (std::string::size_type i = 0; i < token.size(); ++i)
	{
		if (!std::isdigit(static_cast<unsigned char>(token[i])))
			throw InvalidInputException();
	}

	unsigned long	value = std::strtoul(token.c_str(), NULL, 10);

	if (value > static_cast<unsigned long>(INT_MAX))
		throw InvalidInputException();

	return (static_cast<unsigned int>(value));
}

void	PmergeMe::sortVector(int count, char **args)
{
	vec.clear();
	for (int i = 0; i < count; ++i)
	{
		std::istringstream	iss(args[i]);
		std::string			token;

		while (iss >> token)
			vec.push_back(toPositiveInt(token));
	}
	if (vec.empty())
		throw InvalidInputException();

	std::vector<std::size_t>	indices;

	for (std::size_t i = 0; i < vec.size(); ++i)
		indices.push_back(i);
	vecComparisons = 0;
	indices = fordJohnsonVector(vec, indices, vecComparisons);

	std::vector<unsigned int>	sorted;

	for (std::size_t i = 0; i < indices.size(); ++i)
		sorted.push_back(vec[indices[i]]);
	vec = sorted;
}

void	PmergeMe::sortDeque(int count, char **args)
{
	deq.clear();
	for (int i = 0; i < count; ++i)
	{
		std::istringstream	iss(args[i]);
		std::string			token;

		while (iss >> token)
			deq.push_back(toPositiveInt(token));
	}
	if (deq.empty())
		throw InvalidInputException();

	std::deque<std::size_t>	indices;

	for (std::size_t i = 0; i < deq.size(); ++i)
		indices.push_back(i);
	deqComparisons = 0;
	indices = fordJohnsonDeque(deq, indices, deqComparisons);

	std::deque<unsigned int>	sorted;

	for (std::size_t i = 0; i < indices.size(); ++i)
		sorted.push_back(deq[indices[i]]);
	deq = sorted;
}

const std::vector<unsigned int>&	PmergeMe::getVector() const
{
	return (vec);
}

const std::deque<unsigned int>&	PmergeMe::getDeque() const
{
	return (deq);
}

std::size_t	PmergeMe::getVectorComparisons() const
{
	return (vecComparisons);
}

std::size_t	PmergeMe::getDequeComparisons() const
{
	return (deqComparisons);
}

PmergeMe::InvalidInputException::InvalidInputException()
	: std::invalid_argument("Error")
{
}
