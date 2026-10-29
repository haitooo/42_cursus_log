#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

#include <iostream>
#include <string>
#include <map>
#include <stdexcept>

class	BitcoinExchange {
	private:
		std::map<std::string, double>	_rates;

		BitcoinExchange();

		void	loadRates(const std::string& db_path);
		void	validateDBData(const std::string& line) const;
		void	validateInput(const std::string& line) const;
		void	printConversion(const std::string& date, double amount, double rate) const;

	public:
		BitcoinExchange(const std::string& db_path);
		BitcoinExchange(const BitcoinExchange& other);
		BitcoinExchange& operator=(const BitcoinExchange& other);
		~BitcoinExchange();

		void	calcValues(const std::string& input_path);

		class	RunTimeErrorException : public std::runtime_error {
			public:
				RunTimeErrorException();
				RunTimeErrorException(const std::string& message);
		};
};

#endif
