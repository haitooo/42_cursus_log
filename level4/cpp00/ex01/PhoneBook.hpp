#ifndef PHONEBOOK_HPP
# define PHONEBOOK_HPP

# include <iostream>
# include "Contact.hpp"

class PhoneBook
{
	private:
		Contact	_contacts[8];
		int		_contactCount;

	public:
		PhoneBook();
		void	addcontact();
		void	displaycontacts();
};

#endif