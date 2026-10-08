#include "Span.hpp"
#include <algorithm>
#include <numeric>

Span::Span() : numbers(), capacity(0)
{
}

Span::Span(unsigned int n) : numbers(), capacity(n)
{
	numbers.reserve(n);
}

Span::Span(const Span& other) : numbers(other.numbers), capacity(other.capacity)
{
}

Span&	Span::operator=(const Span& other)
{
	if (this != &other)
	{
		numbers = other.numbers;
		capacity = other.capacity;
	}

	return (*this);
}

Span::~Span()
{
}

void	Span::addNumber(int number)
{
	if (numbers.size() >= capacity)
		throw FullException();

	numbers.push_back(number);
}

unsigned int	Span::shortestSpan() const
{
	if (numbers.size() < 2)
		throw NoSpanException();

	std::vector<long>	sorted(numbers.begin(), numbers.end());
	std::vector<long>	diffs(sorted.size());

	std::sort(sorted.begin(), sorted.end());
	std::adjacent_difference(sorted.begin(), sorted.end(), diffs.begin());

	return (static_cast<unsigned int>(*std::min_element(diffs.begin() + 1, diffs.end())));
}

unsigned int	Span::longestSpan() const
{
	if (numbers.size() < 2)
		throw NoSpanException();

	long	minValue = *std::min_element(numbers.begin(), numbers.end());
	long	maxValue = *std::max_element(numbers.begin(), numbers.end());

	return (static_cast<unsigned int>(maxValue - minValue));
}

unsigned int	Span::size() const
{
	return (numbers.size());
}

unsigned int	Span::getCapacity() const
{
	return (capacity);
}

Span::FullException::FullException()
	: std::length_error("Span: no room left for another number")
{
}

Span::NoSpanException::NoSpanException()
	: std::logic_error("Span: at least two numbers are needed to find a span")
{
}
