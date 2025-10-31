#include "Contact.hpp"
#include <iomanip>

#define WIDTH 75

void	Contact::setFirstName(const std::string& str)
{
	_firstName = str;
}

void	Contact::setLastName(const std::string& str)
{
	_lastName = str;
}

void	Contact::setNickName(const std::string& str)
{
	_nickName = str;
}

void	Contact::setPhoneNum(const std::string& str)
{
	_phoneNum = str;
}

void	Contact::setSecret(const std::string& str)
{
	_secret = str;
}

std::string	Contact::getFirstName() const
{
	return (_firstName);
}

std::string	Contact::getLastName() const
{
	return (_lastName);
}

std::string	Contact::getNickName() const
{
	return (_nickName);
}

std::string	Contact::getPhoneNum() const
{
	return (_phoneNum);
}

std::string	Contact::getSecret() const
{
	return (_secret);
}

void Contact::showDetail() const
{
	std::cout << std::endl;
	std::cout << std::setw(WIDTH-20) << "First Name: " << _firstName << std::endl;
	std::cout << std::setw(WIDTH-20) << "Last Name: " << _lastName << std::endl;
	std::cout << std::setw(WIDTH-20) << "Nickname: " << _nickName << std::endl;
	std::cout << std::setw(WIDTH-20) << "Phone Number: " << _phoneNum << std::endl;
	std::cout << std::setw(WIDTH-20) << "Darkest Secret: " << _secret << std::endl << std::endl;
}
