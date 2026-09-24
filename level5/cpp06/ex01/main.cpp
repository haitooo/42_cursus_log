#include "Serializer.hpp"
#include "Data.hpp"
#include "TextFormatter.h"
#include <iostream>

int	main()
{
	Data	data;

	data.i = 42;
	data.str = "Hello World";

	uintptr_t	raw = Serializer::serialize(&data);
	Data*		restored = Serializer::deserialize(raw);

	std::cout << "original : " << &data << std::endl;
	std::cout << "raw      : 0x" << std::hex << raw << std::dec << std::endl;
	std::cout << "restored : " << restored << std::endl;

	if (restored != &data)
	{
		std::cout << RED << "NG: restored pointer differs from the original" << RESET << std::endl;
		return (1);
	}
	std::cout << GREEN << "OK: restored pointer equals the original" << RESET << std::endl;
	std::cout << "restored->i   : " << restored->i << std::endl;
	std::cout << "restored->str : " << restored->str << std::endl;
	return (0);
}
