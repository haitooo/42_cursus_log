#include "PhoneBook.hpp"

PhoneBook::PhoneBook() : _contactCount(0) {}

void	PhoneBook::addcontact()
{
	Contact		contact;
	std::string	input;

	std::cout << "Enter first name     >> ";
	std::cin >> input;
	contact.set_firstName(input);
	std::cout << "Enter last name      >> ";
	std::cin >> input;
	contact.set_lastName(input);
	std::cout << "Enter nickname       >> ";
	std::cin >> input;
	contact.set_nickName(input);
	std::cout << "Enter phone number   >> ";
	std::cin >> input;
	contact.set_phoneNum(input);
	std::cout << "Enter darkest secret >> ";
	std::cin >> input;
	contact.set_secret(input);
	if (this->_contactCount < 8)
		this->_contactCount++;
	this->_contacts[this->_contactCount - 1] = contact;
}

void	PhoneBook::displaycontacts()
{

}
