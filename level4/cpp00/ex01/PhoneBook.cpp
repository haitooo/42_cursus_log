#include "PhoneBook.hpp"
#include <cstdlib> 
#include <iomanip>

#define WIDTH 75

PhoneBook::PhoneBook() : _contactcount(0) {}

bool	is_all_digit(const std::string& str)
{
	size_t	i = 0;

	while (i < str.length())
	{
		if (!std::isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

void	put_colum(std::string str)
{
	if (str.length() > 10)
		std::cout <<str.substr(0, 9) << ".";
	else
		std::cout << std::string(10 - str.length(), ' ') << str;
	std::cout << "|";
}

void	PhoneBook::addcontact()
{
	int			i = -1;
	Contact		contact;
	std::string	input;

	std::cout << std::setw(WIDTH-40) << "Enter first name     >> ";
	std::getline(std::cin, input);
	if (std::cin.eof())
		return ;
	if (input.empty())
	{
		std::cout << std::setw(WIDTH+2) << ">>\033[31m Error: first name cannot be empty.\033[m" << std::endl << std::endl;
		return ;
	}
	contact.set_firstName(input);
	std::cout << std::setw(WIDTH-40) << "Enter last name      >> ";
	std::getline(std::cin, input);
	if (std::cin.eof())
		return ;
	if (input.empty()) {
		std::cout << std::setw(WIDTH+2) << ">>\033[31m Error: last name cannot be empty.\033[m" << std::endl << std::endl;
		return ;
	}
	contact.set_lastName(input);
	std::cout << std::setw(WIDTH-40) << "Enter nickname       >> ";
	std::getline(std::cin, input);
	if (std::cin.eof())
		return ;
	if (input.empty())
	{
		std::cout << std::setw(WIDTH+2) << ">>\033[31m Error: nickname cannot be empty.\033[m" << std::endl << std::endl;
		return ;
	}
	contact.set_nickName(input);
	std::cout << std::setw(WIDTH-40) << "Enter phone number   >> ";
	std::getline(std::cin, input);
	if (std::cin.eof())
		return ;
	if (input.empty())
	{
		std::cout << std::setw(WIDTH+2) << ">>\033[31m Error: phone number cannot be empty.\033[m" << std::endl << std::endl;
		return ;
	}
	contact.set_phoneNum(input);
	std::cout << std::setw(WIDTH-40) << "Enter darkest secret >> ";
	std::getline(std::cin, input);
	if (std::cin.eof())
		return ;
	if (input.empty())
	{
		std::cout << std::setw(WIDTH+2) << ">>\033[31m Error: secret cannot be empty.\033[m" << std::endl << std::endl;
		return ;
	}
	contact.set_secret(input);
	std::cout << std::endl;
	if (_contactcount < 8)
	{
		_contactcount++;
		_contacts[_contactcount - 1] = contact;
	}
	else
	{
		while (++i < 7)
			_contacts[i] = _contacts[i + 1];
		_contacts[7] = contact;
	}
}

void	PhoneBook::displaycontacts()
{
	int			i = -1;
	int			index;
	std::string	str;
	std::string	input;

	std::cout << std::endl;
	std::cout << std::setw(WIDTH) << "|     Index|First Name| Last Name|  Nickname|" << std::endl;
	while (++i < _contactcount)
	{
		std::cout << std::setw(WIDTH-35) << "|         " << i + 1 << "|";
		str = _contacts[i].get_firstName();
		put_colum(str);
		str = _contacts[i].get_lastName();
		put_colum(str);
		str = _contacts[i].get_nickName();
		put_colum(str);
		std::cout << std::endl;
	}
	std::cout << std::endl;
	while (true)
	{
		std::cout << std::setw(WIDTH-40) << "Enter index         >> ";
		std::getline(std::cin, input);
		if (std::cin.eof())
			return ;
		if (!is_all_digit(input))
		{
			std::cout << std::setw(WIDTH) << "\033[31mInvalid input. Please enter (1 - "<< PhoneBook::_contactcount << ")\033[m" << std::endl;
			continue ;
		}
		index = std::atoi(input.c_str());
		if (index < 1 || index > _contactcount)
		{
			std::cout << std::setw(WIDTH) << "\033[31mIndex out of range. Try again.\033[m" << std::endl;
			continue ;
		}
		_contacts[index - 1].show_detail();
		break ;
	}
}
