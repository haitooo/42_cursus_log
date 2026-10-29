#include "BitcoinExchange.hpp"

#define DATA_PATH "data.csv"

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Error: could not open file." << std::endl;
		return (1);
	}

	try
	{
		BitcoinExchange	exchange(DATA_PATH);

		exchange.calcValues(argv[1]);
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return (1);
	}

	return (0);
}
