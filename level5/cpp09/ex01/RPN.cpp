#include "RPN.hpp"
#include <sstream>
#include <cctype>

RPN::RPN(const std::string& input) : input(input), numbers()
{
}

RPN::RPN(const RPN& other) : input(other.input), numbers(other.numbers)
{
}

RPN& RPN::operator=(const RPN& other)
{
	if (this != &other)
	{
		input = other.input;
		numbers = other.numbers;
	}

	return (*this);
}

RPN::~RPN()
{
}

void	RPN::applyOperator(char op)
{
	if (numbers.size() < 2)
		throw RunTimeErrorException("Error: not enough operands for '" + std::string(1, op) + "'");

	long	right = numbers.back();
	numbers.pop_back();
	long	left = numbers.back();
	numbers.pop_back();

	switch (op)
	{
		case '+':
			numbers.push_back(left + right);
			break ;
		case '-':
			numbers.push_back(left - right);
			break ;
		case '*':
			numbers.push_back(left * right);
			break ;
		case '/':
			if (right == 0)
				throw RunTimeErrorException("Error: division by zero");
			numbers.push_back(left / right);
			break ;
	}
}

long	RPN::calculate()
{
	std::istringstream	iss(input);
	std::string			token;

	numbers.clear();

	while (iss >> token)
	{
		if (token.size() != 1)
			throw RunTimeErrorException("Error: invalid token '" + token + "'");

		char	c = token[0];

		if (std::isdigit(static_cast<unsigned char>(c)))
			numbers.push_back(c - '0');
		else if (c == '+' || c == '-' || c == '*' || c == '/')
			applyOperator(c);
		else
			throw RunTimeErrorException("Error: invalid token '" + token + "'");
	}

	if (numbers.size() != 1)
		throw RunTimeErrorException("Error: invalid expression");

	return (numbers.back());
}

RPN::RunTimeErrorException::RunTimeErrorException()
	: std::runtime_error("Error: runtime error.")
{
}

RPN::RunTimeErrorException::RunTimeErrorException(const std::string& message)
	: std::runtime_error(message)
{
}
