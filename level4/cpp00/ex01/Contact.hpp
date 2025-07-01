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
	void		show_detail() const;
	void		set_firstName(const std::string&);
	void		set_lastName(const std::string&);
	void		set_nickName(const std::string&);
	void		set_phoneNum(const std::string&);
	void		set_secret(const std::string&);
	std::string	get_firstName() const;
	std::string	get_lastName() const;
	std::string	get_nickName() const;
	std::string	get_phoneNum() const;
	std::string	get_secret() const;
};

#endif