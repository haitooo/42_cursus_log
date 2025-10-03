#include "Contact.hpp"
#include <iomanip>

#define WIDTH 75

void	Contact::set_firstName(const std::string& str)
{
	_firstName = str;
}

void	Contact::set_lastName(const std::string& str)
{
	_lastName = str;
}

void	Contact::set_nickName(const std::string& str)
{
	_nickName = str;
}

void	Contact::set_phoneNum(const std::string& str)
{
	_phoneNum = str;
}

void	Contact::set_secret(const std::string& str)
{
	_secret = str;
}

std::string	Contact::get_firstName() const
{
	return (_firstName);
}

std::string	Contact::get_lastName() const
{
	return (_lastName);
}

std::string	Contact::get_nickName() const
{
	return (_nickName);
}

std::string	Contact::get_phoneNum() const
{
	return (_phoneNum);
}

std::string	Contact::get_secret() const
{
	return (_secret);
}

void Contact::show_detail() const
{
	std::cout << std::endl;
	std::cout << std::setw(WIDTH-20) << "First Name: " << _firstName << std::endl;
	std::cout << std::setw(WIDTH-20) << "Last Name: " << _lastName << std::endl;
	std::cout << std::setw(WIDTH-20) << "Nickname: " << _nickName << std::endl;
	std::cout << std::setw(WIDTH-20) << "Phone Number: " << _phoneNum << std::endl;
	std::cout << std::setw(WIDTH-20) << "Darkest Secret: " << _secret << std::endl << std::endl;
}

