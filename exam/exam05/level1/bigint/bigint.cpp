#include "bigint.hpp"

bigint::bigint() : _value("0")
{
}

bigint::bigint(unsigned int ui)
{
	std::ostringstream oss;
	oss << ui;
	_value = oss.str();
}

bigint::bigint(std::string str) : _value(str)
{
}

bigint::bigint(const bigint& other) : _value(other.getValue())
{
}

bigint&	bigint::operator=(const bigint& other)
{
	if (this != &other)
		_value = other.getValue();
	return (*this);
}

bigint::~bigint()
{
}

std::string bigint::getValue() const
{
	return (_value);
}

bigint	bigint::operator+(const bigint& other) const
{
	std::string	a = getValue();
	std::string	b = other.getValue();
	std::string	result;

	std::string::size_type	asize = a.size();
	std::string::size_type	bsize = b.size();

	std::string::size_type	i = 1;
	int	carry = 0;
	while (i <= asize || i <= bsize || carry != 0)
	{
		int	sum = carry;
		if (i <= asize)
			sum += (a[asize - i] - '0');
		if (i <= bsize)
			sum += (b[bsize - i] - '0');
		result += static_cast<char>(sum % 10 + '0');
		carry = sum / 10;
		i++;
	}
	std::reverse(result.begin(), result.end());
	return (bigint(result));
}

bigint	bigint::operator+(unsigned int n) const
{
	std::string	a = getValue();
	std::ostringstream oss;
	oss << n;
	std::string	b = oss.str();
	std::string	result;

	std::string::size_type	asize = a.size();
	std::string::size_type	bsize = b.size();

	std::string::size_type	i = 1;
	int	carry = 0;
	while (i <= asize || i <= bsize || carry != 0)
	{
		int	sum = carry;
		if (i <= asize)
			sum += (a[asize - i] - '0');
		if (i <= bsize)
			sum += (b[bsize - i] - '0');
		result += static_cast<char>(sum % 10 + '0');
		carry = sum / 10;
		i++;
	}
	std::reverse(result.begin(), result.end());
	return (bigint(result));
}

bigint&	bigint::operator+=(const bigint& other)
{
	std::string	a = getValue();
	std::string	b = other.getValue();
	std::string	result;

	std::string::size_type	asize = a.size();
	std::string::size_type	bsize = b.size();

	std::string::size_type	i = 1;
	int	carry = 0;
	while (i <= asize || i <= bsize || carry != 0)
	{
		int	sum = carry;
		if (i <= asize)
			sum += (a[asize - i] - '0');
		if (i <= bsize)
			sum += (b[bsize - i] - '0');
		result += static_cast<char>(sum % 10 + '0');
		carry = sum / 10;
		i++;
	}
	std::reverse(result.begin(), result.end());
	bigint tmp(result);
	*this = tmp;
	return (*this);
}

bigint&	bigint::operator+=(unsigned int n)
{
	std::string	a = getValue();
	std::ostringstream oss;
	oss << n;
	std::string	b = oss.str();
	std::string	result;

	std::string::size_type	asize = a.size();
	std::string::size_type	bsize = b.size();

	std::string::size_type	i = 1;
	int	carry = 0;
	while (i <= asize || i <= bsize || carry != 0)
	{
		int	sum = carry;
		if (i <= asize)
			sum += (a[asize - i] - '0');
		if (i <= bsize)
			sum += (b[bsize - i] - '0');
		result += static_cast<char>(sum % 10 + '0');
		carry = sum / 10;
		i++;
	}
	std::reverse(result.begin(), result.end());
	bigint tmp(result);
	*this = tmp;
	return (*this);
}

bigint&	bigint::operator++()
{
	return (*this += 1);
}

bigint	bigint::operator++(int)
{
	bigint tmp(*this);
	++*this;
	return (tmp);
}

bigint	bigint::operator<<(unsigned int n) const
{
	std::string	tmp = getValue();
	if (tmp != "0")
		tmp.append(n, '0');
	return (bigint(tmp));
}

bigint	bigint::operator<<(const bigint& other) const
{
	std::string	tmp = getValue();
	std::istringstream		iss(other.getValue());
	std::string::size_type	shift = 0;

	if (!(iss >> shift))
		return (bigint(tmp));

	if (tmp != "0")
		tmp.append(shift, '0');
	return (bigint(tmp));
}

bigint	bigint::operator>>(unsigned int n) const
{
	std::string	tmp = getValue();
	if (tmp.size() <= n)
		return (bigint("0"));
	tmp.resize(tmp.size() - n);
	return (bigint(tmp));
}

bigint	bigint::operator>>(const bigint& other) const
{
	std::string	tmp = getValue();
	std::istringstream		iss(other.getValue());
	std::string::size_type	shift = 0;

	if (!(iss >> shift) || shift >= tmp.size())
		return (bigint(0));
	tmp.resize(tmp.size() - shift);
	return (bigint(tmp));
}

bigint&	bigint::operator<<=(unsigned int n)
{
	*this = *this << n;
	return (*this);
}

bigint&	bigint::operator>>=(unsigned int n)
{
	*this = *this >> n;
	return (*this);
}

bigint&	bigint::operator>>=(const bigint& other)
{
	*this = *this >> other;
	return (*this);
}

bool	bigint::operator<(const bigint& other) const
{
	if (getValue().size() != other.getValue().size())
		return (getValue().size() < other.getValue().size());
	return (getValue() < other.getValue());
}

bool	bigint::operator>(const bigint& other) const
{
	if (getValue().size() != other.getValue().size())
		return (getValue().size() > other.getValue().size());
	return (getValue() > other.getValue());
}

bool	bigint::operator<=(const bigint& other) const
{
	if (getValue() == other.getValue())
		return (true);
	if (getValue().size() != other.getValue().size())
		return (getValue().size() < other.getValue().size());
	return (getValue() < other.getValue());
}

bool	bigint::operator>=(const bigint& other) const
{
	if (getValue() == other.getValue())
		return (true);
	if (getValue().size() != other.getValue().size())
		return (getValue().size() > other.getValue().size());
	return (getValue() > other.getValue());
}

bool	bigint::operator==(const bigint& other) const
{
	return (getValue() == other.getValue());
}

bool	bigint::operator!=(const bigint& other) const
{
	return (!(getValue() == other.getValue()));
}


std::ostream&	operator<<(std::ostream& os, const bigint& bi)
{
	os << bi.getValue();
	return (os);
}
