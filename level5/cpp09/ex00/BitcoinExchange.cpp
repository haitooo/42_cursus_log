#include "BitcoinExchange.hpp"
#include <fstream>
#include <cstdlib>
#include <cctype>

static const std::string	DB_HEADER = "date,exchange_rate";
static const std::string	INPUT_HEADER = "date | value";
static const std::string	INPUT_SEPARATOR = " | ";

static bool	isLeapYear(int year)
{
	return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0);
}

static bool	isValidDate(const std::string& date)
{
	if (date.size() != 10 || date[4] != '-' || date[7] != '-')
		return (false);
	for (std::string::size_type i = 0; i < date.size(); ++i)
	{
		if (i == 4 || i == 7)
			continue ;
		if (!std::isdigit(static_cast<unsigned char>(date[i])))
			return (false);
	}

	int	year = std::atoi(date.substr(0, 4).c_str());
	int	month = std::atoi(date.substr(5, 2).c_str());
	int	day = std::atoi(date.substr(8, 2).c_str());
	int	daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	if (month < 1 || month > 12)
		return (false);
	if (month == 2 && isLeapYear(year))
		daysInMonth[1] = 29;

	return (day >= 1 && day <= daysInMonth[month - 1]);
}

static bool	isNumber(const std::string& str)
{
	std::string::size_type	i = 0;
	bool					hasDigit = false;
	bool					hasDot = false;

	if (i < str.size() && (str[i] == '+' || str[i] == '-'))
		++i;
	for (; i < str.size(); ++i)
	{
		if (std::isdigit(static_cast<unsigned char>(str[i])))
			hasDigit = true;
		else if (str[i] == '.' && !hasDot)
			hasDot = true;
		else
			return (false);
	}

	return (hasDigit);
}

BitcoinExchange::BitcoinExchange(const std::string& db_path) : _rates()
{
	loadRates(db_path);
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) : _rates(other._rates)
{
}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other)
{
	if (this != &other)
		_rates = other._rates;

	return (*this);
}

BitcoinExchange::~BitcoinExchange()
{
}

void	BitcoinExchange::loadRates(const std::string& db_path)
{
	std::ifstream	file(db_path.c_str());
	std::string		line;

	if (!file.is_open())
		throw RunTimeErrorException("Error: could not open database file.");

	if (!std::getline(file, line) || line != DB_HEADER)
		throw RunTimeErrorException("Error: invalid database header.");

	while (std::getline(file, line))
	{
		validateDBData(line);

		std::string::size_type	comma = line.find(',');

		_rates[line.substr(0, comma)] = std::strtod(line.substr(comma + 1).c_str(), NULL);
	}

	if (_rates.empty())
		throw RunTimeErrorException("Error: database is empty.");
}

void	BitcoinExchange::validateDBData(const std::string& line) const
{
	std::string::size_type	comma = line.find(',');

	if (comma == std::string::npos
		|| !isValidDate(line.substr(0, comma))
		|| !isNumber(line.substr(comma + 1))
		|| std::strtod(line.substr(comma + 1).c_str(), NULL) < 0)
		throw RunTimeErrorException("Error: invalid database line => " + line);
}

void	BitcoinExchange::validateInput(const std::string& line) const
{
	std::string::size_type	sep = line.find(INPUT_SEPARATOR);

	if (sep == std::string::npos)
		throw RunTimeErrorException("Error: bad input => " + line);

	std::string	date = line.substr(0, sep);
	std::string	valueStr = line.substr(sep + INPUT_SEPARATOR.size());

	if (!isValidDate(date) || !isNumber(valueStr))
		throw RunTimeErrorException("Error: bad input => " + line);

	double	value = std::strtod(valueStr.c_str(), NULL);

	if (value < 0)
		throw RunTimeErrorException("Error: not a positive number.");
	if (value > 1000)
		throw RunTimeErrorException("Error: too large a number.");
	if (date < _rates.begin()->first)
		throw RunTimeErrorException("Error: no rate before this date => " + date);
}

void	BitcoinExchange::printConversion(const std::string& date, double amount, double rate) const
{
	std::cout << date << " => " << amount << " = " << amount * rate << std::endl;
}

void	BitcoinExchange::calcValues(const std::string& input_path)
{
	std::ifstream	file(input_path.c_str());
	std::string		line;

	if (!file.is_open())
		throw RunTimeErrorException("Error: could not open file.");

	if (!std::getline(file, line) || line != INPUT_HEADER)
		throw RunTimeErrorException("Error: invalid input header.");

	while (std::getline(file, line))
	{
		try
		{
			validateInput(line);
		}
		catch (std::exception& e)
		{
			std::cout << e.what() << std::endl;
			continue ;
		}

		std::string::size_type	sep = line.find(INPUT_SEPARATOR);
		std::string				date = line.substr(0, sep);
		double					amount = std::strtod(line.substr(sep + INPUT_SEPARATOR.size()).c_str(), NULL);

		std::map<std::string, double>::const_iterator	it = _rates.upper_bound(date);

		--it;
		printConversion(date, amount, it->second);
	}
}

BitcoinExchange::RunTimeErrorException::RunTimeErrorException()
	: std::runtime_error("Error: runtime error.")
{
}

BitcoinExchange::RunTimeErrorException::RunTimeErrorException(const std::string& message)
	: std::runtime_error(message)
{
}
