#include "ScalarConverter.hpp"
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>

ScalarConverter::ScalarConverter()
{
}

ScalarConverter::ScalarConverter(const ScalarConverter& other)
{
	(void)other;
}

ScalarConverter&	ScalarConverter::operator=(const ScalarConverter& other)
{
	(void)other;
	return (*this);
}

ScalarConverter::~ScalarConverter()
{
}

void	ScalarConverter::convert(const std::string& literal)
{
	switch (detectType(literal))
	{
		case TYPE_CHAR:
			convertFromChar(parseChar(literal));
			break ;
		case TYPE_INT:
			convertFromInt(parseInt(literal));
			break ;
		case TYPE_FLOAT:
			convertFromFloat(parseFloat(literal));
			break ;
		case TYPE_DOUBLE:
			convertFromDouble(parseDouble(literal));
			break ;
		default:
			printImpossible("char");
			printImpossible("int");
			printImpossible("float");
			printImpossible("double");
			break ;
	}
}

ScalarConverter::LiteralType	ScalarConverter::detectType(const std::string& literal)
{
	if (literal.length() == 3 && literal[0] == '\'' && literal[2] == '\'')
		return (TYPE_CHAR);
	if (literal.length() == 1 && !std::isdigit(static_cast<unsigned char>(literal[0])))
		return (TYPE_CHAR);
	if (literal == "nanf" || literal == "+inff" || literal == "-inff" || literal == "inff")
		return (TYPE_FLOAT);
	if (literal == "nan" || literal == "+inf" || literal == "-inf" || literal == "inf")
		return (TYPE_DOUBLE);

	size_t	i = 0;
	bool	hasDigit = false;
	bool	hasDot = false;

	if (i < literal.length() && (literal[i] == '+' || literal[i] == '-'))
		i++;
	for (; i < literal.length(); i++)
	{
		if (std::isdigit(static_cast<unsigned char>(literal[i])))
			hasDigit = true;
		else if (literal[i] == '.' && !hasDot)
			hasDot = true;
		else
			break ;
	}
	if (!hasDigit)
		return (TYPE_INVALID);
	if (i == literal.length() && !hasDot)
		return (checkRange(literal, TYPE_INT));
	if (i == literal.length())
		return (checkRange(literal, TYPE_DOUBLE));
	if (hasDot && literal[i] == 'f' && i + 1 == literal.length())
		return (checkRange(literal, TYPE_FLOAT));
	return (TYPE_INVALID);
}

ScalarConverter::LiteralType	ScalarConverter::checkRange(const std::string& literal, LiteralType type)
{
	double	value = std::strtod(literal.c_str(), NULL);

	if (isInf(value))
		return (TYPE_INVALID);
	if (type == TYPE_INT && !fitsInt(value))
		return (TYPE_DOUBLE);
	if (type == TYPE_FLOAT && !fitsFloat(value))
		return (TYPE_INVALID);
	return (type);
}

char	ScalarConverter::parseChar(const std::string& literal)
{
	if (literal.length() == 3)
		return (literal[1]);
	return (literal[0]);
}

int	ScalarConverter::parseInt(const std::string& literal)
{
	return (static_cast<int>(std::strtol(literal.c_str(), NULL, 10)));
}

float	ScalarConverter::parseFloat(const std::string& literal)
{
	if (literal == "nanf")
		return (std::numeric_limits<float>::quiet_NaN());
	if (literal == "-inff")
		return (-std::numeric_limits<float>::infinity());
	if (literal == "+inff" || literal == "inff")
		return (std::numeric_limits<float>::infinity());
	return (static_cast<float>(std::strtod(literal.c_str(), NULL)));
}

double	ScalarConverter::parseDouble(const std::string& literal)
{
	if (literal == "nan")
		return (std::numeric_limits<double>::quiet_NaN());
	if (literal == "-inf")
		return (-std::numeric_limits<double>::infinity());
	if (literal == "+inf" || literal == "inf")
		return (std::numeric_limits<double>::infinity());
	return (std::strtod(literal.c_str(), NULL));
}

void	ScalarConverter::convertFromChar(char c)
{
	printChar(c);
	printInt(static_cast<int>(c));
	printFloat(static_cast<float>(c));
	printDouble(static_cast<double>(c));
}

void	ScalarConverter::convertFromInt(int n)
{
	if (fitsChar(n))
		printChar(static_cast<char>(n));
	else
		printImpossible("char");
	printInt(n);
	printFloat(static_cast<float>(n));
	printDouble(static_cast<double>(n));
}

void	ScalarConverter::convertFromFloat(float f)
{
	if (fitsChar(f))
		printChar(static_cast<char>(f));
	else
		printImpossible("char");
	if (fitsInt(f))
		printInt(static_cast<int>(f));
	else
		printImpossible("int");
	printFloat(f);
	printDouble(static_cast<double>(f));
}

void	ScalarConverter::convertFromDouble(double d)
{
	if (fitsChar(d))
		printChar(static_cast<char>(d));
	else
		printImpossible("char");
	if (fitsInt(d))
		printInt(static_cast<int>(d));
	else
		printImpossible("int");
	if (fitsFloat(d))
		printFloat(static_cast<float>(d));
	else
		printImpossible("float");
	printDouble(d);
}

void	ScalarConverter::printChar(char c)
{
	if (std::isprint(static_cast<unsigned char>(c)))
		std::cout << "char: '" << c << "'" << std::endl;
	else
		std::cout << "char: Non displayable" << std::endl;
}

void	ScalarConverter::printInt(int n)
{
	std::cout << "int: " << n << std::endl;
}

void	ScalarConverter::printFloat(float f)
{
	std::cout << "float: "
		<< formatDecimal(f, std::numeric_limits<float>::digits10) << "f" << std::endl;
}

void	ScalarConverter::printDouble(double d)
{
	std::cout << "double: "
		<< formatDecimal(d, std::numeric_limits<double>::digits10) << std::endl;
}

void	ScalarConverter::printImpossible(const char* typeName)
{
	std::cout << typeName << ": impossible" << std::endl;
}

bool	ScalarConverter::fitsChar(double value)
{
	return (!isNan(value)
		&& value >= std::numeric_limits<char>::min()
		&& value <= std::numeric_limits<char>::max());
}

bool	ScalarConverter::fitsInt(double value)
{
	return (!isNan(value)
		&& value >= std::numeric_limits<int>::min()
		&& value <= std::numeric_limits<int>::max());
}

bool	ScalarConverter::fitsFloat(double value)
{
	return (isNan(value) || isInf(value)
		|| std::fabs(value) <= std::numeric_limits<float>::max());
}

bool	ScalarConverter::isNan(double value)
{
	return (value != value);
}

bool	ScalarConverter::isInf(double value)
{
	return (value == std::numeric_limits<double>::infinity()
		|| value == -std::numeric_limits<double>::infinity());
}

std::string	ScalarConverter::formatDecimal(double value, int precision)
{
	if (isNan(value))
		return ("nan");
	if (isInf(value))
		return (value > 0 ? "+inf" : "-inf");

	std::ostringstream	oss;

	if (value == std::floor(value) && std::fabs(value) < 1e16)
		oss << std::fixed << std::setprecision(1) << value;
	else
		oss << std::setprecision(precision) << value;
	return (oss.str());
}
