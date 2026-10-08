#ifndef SPAN_HPP
# define SPAN_HPP

#include <iterator>
#include <stdexcept>
#include <vector>

class	Span {
	private:
		std::vector<int>	numbers;
		unsigned int		capacity;

	public:
		Span();
		Span(unsigned int n);
		Span(const Span& other);
		Span&	operator=(const Span& other);
		~Span();

		void			addNumber(int number);
		template <typename InputIterator>
		void			addNumber(InputIterator first, InputIterator last);
		unsigned int	shortestSpan() const;
		unsigned int	longestSpan() const;
		unsigned int	size() const;
		unsigned int	getCapacity() const;

		class	FullException : public std::length_error {
			public:
				FullException();
		};
		class	NoSpanException : public std::logic_error {
			public:
				NoSpanException();
		};
};

template <typename InputIterator>
void	Span::addNumber(InputIterator first, InputIterator last)
{
	if (static_cast<unsigned long>(std::distance(first, last)) > capacity - numbers.size())
		throw FullException();

	numbers.insert(numbers.end(), first, last);
}

#endif
