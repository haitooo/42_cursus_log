#include <iostream>
#include <string>
#include <iomanip>
#include "Contact.hpp"
#include "PhoneBook.hpp"

#define WIDTH 75

void	put_guide(void)
{
	std::cout << std::setw(WIDTH+18) << "Please enter \033[1mADD\033[m or \033[1mSEARCH\033[m or \033[1mEXIT\033[m" << std::endl << std::endl;
	std::cout << std::setw(WIDTH-10) << "\033[32m<HELP>\033[m" << std::endl;
	std::cout << std::setw(WIDTH+31) << "\033[1;4mADD\033[m\033[3;4m Save a new contact. A maximum of 8 contacts can be saved.\033[m" << std::endl;
	std::cout << std::setw(WIDTH+39) << "\033[1;4mSEARCH\033[m\033[3;4m Show a list of contacts. Enter an index to view detailed information.\033[m" << std::endl;
	std::cout << std::setw(WIDTH+14) << "\033[1;4mEXIT\033[m\033[3;4m Exit the PhoneBook program.\033[m\n" << std::endl;
	std::cout << std::setw(WIDTH-40) << ">> ";
}

int	main(void)
{
	int				i;
	std::string		input;
	PhoneBook		PB;

	i = 0;
	while (i++ < 20)
		std::cout << " ";
	while (i++ < 90)
		std::cout << "\033[1;34m-\033[m";
	std::cout << std::endl << std::endl;
	std::cout << std::setw(WIDTH+3) << "\033[1;5;36mWelcome to PhoneBook!\033[m" << std::endl;
	while (true)
	{
		put_guide();
		std::getline(std::cin >> std::ws, input);
		if (input == "ADD")
			PB.addcontact();
		else if (input == "SEARCH")
			PB.displaycontacts();
		else if (input == "EXIT")
		{
			std::cout << std::setw(WIDTH-6) << "\033[1;36mGood bye !\033[m" << std::endl;
			return (0);
		}
	}
	return (0);
}
