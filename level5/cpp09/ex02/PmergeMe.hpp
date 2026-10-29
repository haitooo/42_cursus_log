#ifndef PMERGEME_HPP
# define PMERGEME_HPP

#include <cstddef>
#include <deque>
#include <stdexcept>
#include <string>
#include <vector>

class	PmergeMe {
	private:
		std::vector<unsigned int>	vec;
		std::deque<unsigned int>	deq;
		std::size_t					vecComparisons;
		std::size_t					deqComparisons;

		static unsigned int	toPositiveInt(const std::string& token);

	public:
		PmergeMe();
		PmergeMe(const PmergeMe& other);
		PmergeMe&	operator=(const PmergeMe& other);
		~PmergeMe();

		void	sortVector(int count, char **args);
		void	sortDeque(int count, char **args);

		const std::vector<unsigned int>&	getVector() const;
		const std::deque<unsigned int>&		getDeque() const;
		std::size_t							getVectorComparisons() const;
		std::size_t							getDequeComparisons() const;

		class	InvalidInputException : public std::invalid_argument {
			public:
				InvalidInputException();
		};
};

#endif
