#include "Contact.hpp"
#include <iomanip>

#define WIDTH 75

void	Contact::set_firstName(const std::string& str)
{
	this->_firstName = str;
}

void	Contact::set_lastName(const std::string& str)
{
	this->_lastName = str;
}

void	Contact::set_nickName(const std::string& str)
{
	this->_nickName = str;
}

void	Contact::set_phoneNum(const std::string& str)
{
	this->_phoneNum = str;
}

void	Contact::set_secret(const std::string& str)
{
	this->_secret = str;
}

std::string	Contact::get_firstName() const
{
	return (this->_firstName);
}

std::string	Contact::get_lastName() const
{
	return (this->_lastName);
}

std::string	Contact::get_nickName() const
{
	return (this->_nickName);
}

std::string	Contact::get_phoneNum() const
{
	return (this->_phoneNum);
}

std::string	Contact::get_secret() const
{
	return (this->_secret);
}

void Contact::show_detail() const
{
	std::cout << std::endl;
	std::cout << std::setw(WIDTH-20) << "First Name: " << this->_firstName << std::endl;
	std::cout << std::setw(WIDTH-20) << "Last Name: " << this->_lastName << std::endl;
	std::cout << std::setw(WIDTH-20) << "Nickname: " << this->_nickName << std::endl;
	std::cout << std::setw(WIDTH-20) << "Phone Number: " << this->_phoneNum << std::endl;
	std::cout << std::setw(WIDTH-20) << "Darkest Secret: " << this->_secret << std::endl << std::endl;
}

