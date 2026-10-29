#ifndef RPN_HPP
# define RPN_HPP

#include <iostream>
#include <string>
#include <stack>
#include <list>
#include <stdexcept>

class	RPN {
	private:
		std::string							input;
		std::stack<long, std::list<long> >	numbers;

		RPN();

		void	applyOperator(char op);

	public:
		RPN(const std::string& input);
		RPN(const RPN& other);
		RPN& operator=(const RPN& other);
		~RPN();

		long	calculate();

		class	RunTimeErrorException : public std::runtime_error {
			public:
				RunTimeErrorException();
				RunTimeErrorException(const std::string& message);
		};
	};

#endif
