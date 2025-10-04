#ifndef CONTACT_HPP
# define CONTACT_HPP

# include <iostream>

class Contact
{
	private:
		std::string _firstName;
		std::string _lastName;
		std::string _nickName;
		std::string _phoneNum;
		std::string _secret;

	public:
	void		showDetail() const;
	void		setFirstName(const std::string&);
	void		setLastName(const std::string&);
	void		setNickName(const std::string&);
	void		setPhoneNum(const std::string&);
	void		setSecret(const std::string&);
	std::string	getFirstName() const;
	std::string	getLastName() const;
	std::string	getNickName() const;
	std::string	getPhoneNum() const;
	std::string	getSecret() const;
};

#endif