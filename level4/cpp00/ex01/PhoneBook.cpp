#include "PhoneBook.hpp"
#include <cstdlib> 
#include <iomanip>

#define WIDTH 75

PhoneBook::PhoneBook() : _contactCount(0) {}

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
	std::getline(std::cin >> std::ws, input);
	contact.set_firstName(input);
	std::cout << std::setw(WIDTH-40) << "Enter last name      >> ";
	std::getline(std::cin >> std::ws, input);
	contact.set_lastName(input);
	std::cout << std::setw(WIDTH-40) << "Enter nickname       >> ";
	std::getline(std::cin >> std::ws, input);
	contact.set_nickName(input);
	std::cout << std::setw(WIDTH-40) << "Enter phone number   >> ";
	std::getline(std::cin >> std::ws, input);
	contact.set_phoneNum(input);
	std::cout << std::setw(WIDTH-40) << "Enter darkest secret >> ";
	std::getline(std::cin >> std::ws, input);
	contact.set_secret(input);
	if (this->_contactCount < 8)
	{
		this->_contactCount++;
		this->_contacts[this->_contactCount - 1] = contact;
	}
	else
	{
		while (++i < 7)
			this->_contacts[i] = this->_contacts[i + 1];
		this->_contacts[7] = contact;
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
	while (++i < PhoneBook::_contactCount)
	{
		std::cout << std::setw(WIDTH-35) << "|         " << i + 1 << "|";
		str = PhoneBook::_contacts[i].get_firstName();
		put_colum(str);
		str = PhoneBook::_contacts[i].get_lastName();
		put_colum(str);
		str = PhoneBook::_contacts[i].get_nickName();
		put_colum(str);
		std::cout << std::endl;
	}
	std::cout << std::endl;
	while (true)
	{
		std::cout << std::setw(WIDTH-40) << "Enter index         >> ";
		std::getline(std::cin >> std::ws, input);
		if (!is_all_digit(input))
		{
			std::cout << std::setw(WIDTH-5) << "Invalid input. Please enter (1 - "<< PhoneBook::_contactCount << ")" << std::endl;
			continue ;
		}
		index = std::atoi(input.c_str());
		if (index < 1 || index > this->_contactCount)
		{
			std::cout << std::setw(WIDTH-5) << "Index out of range. Try again." << std::endl;
			continue ;
		}
		this->_contacts[index - 1].show_detail();
		break ;
	}
}
