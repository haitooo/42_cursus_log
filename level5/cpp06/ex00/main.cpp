#include "ScalarConverter.hpp"
#include "TextFormatter.h"
#include <iostream>

int	main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cerr << RED << "Error: expected exactly one argument" << RESET << std::endl;
		std::cerr << "Usage: " << argv[0] << " <literal>" << std::endl;
		std::cerr << "Example: " << argv[0] << " 42 | 4.2f | 4.2 | nanf | -inf" << std::endl;
		return (1);
	}
	ScalarConverter::convert(argv[1]);
	return (0);
}
