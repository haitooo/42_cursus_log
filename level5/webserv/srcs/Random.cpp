#include "Random.hpp"

#include <cstdio>
#include <fstream>

std::string	randomHexToken(std::size_t bytes)
{
	static const char	hexDigits[] = "0123456789abcdef";

	if (bytes == 0)
		return (std::string());

	std::ifstream	urandom("/dev/urandom", std::ios::in | std::ios::binary);

	if (!urandom)
		return (std::string());

	std::string	token;

	token.reserve(bytes * 2);
	for (std::size_t i = 0; i < bytes; ++i)
	{
		const int	value = urandom.get();

		if (value == EOF)
			return (std::string());

		const unsigned char	byte = static_cast<unsigned char>(value);

		token += hexDigits[byte >> 4];
		token += hexDigits[byte & 0x0f];
	}
	return (token);
}
