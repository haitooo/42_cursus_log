#ifndef BIGINT_HPP
# define BIGINT_HPP

#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>

class bigint {
	private:
		std::string	_value;

	public:
		bigint();
		bigint(unsigned int ui);
		bigint(std::string str);
		bigint(const bigint& other);
		bigint&	operator=(const bigint& other);
		~bigint();

		std::string	getValue() const;

		bigint	operator+(const bigint& other) const;
		bigint	operator+(unsigned int n) const;
		bigint&	operator+=(const bigint& other);
		bigint&	operator+=(unsigned int n);
		bigint&	operator++();
		bigint	operator++(int);

		bigint	operator<<(unsigned int n) const;
		bigint	operator<<(const bigint& other) const;
		bigint	operator>>(unsigned int n) const;
		bigint	operator>>(const bigint& other) const;
		bigint&	operator<<=(unsigned int n);
		bigint&	operator>>=(unsigned int n);
		bigint&	operator>>=(const bigint& other);

		bool	operator<(const bigint& other) const;
		bool	operator>(const bigint& other) const;
		bool	operator<=(const bigint& other) const;
		bool	operator>=(const bigint& other) const;
		bool	operator==(const bigint& other) const;
		bool	operator!=(const bigint& other) const;
};

std::ostream&	operator<<(std::ostream& os, const bigint& bi);

#endif
