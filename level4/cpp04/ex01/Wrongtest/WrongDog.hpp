#ifndef WRONGDOG_HPP
# define WRONGDOG_HPP

#include "WrongAnimal.hpp"
#include "TextFormatter.h"

class	WrongDog : public WrongAnimal
{
	public:
		WrongDog();
		WrongDog(const WrongDog& other);
		WrongDog& operator=(const WrongDog& other);
		~WrongDog();

		void	makeSound() const;
};

#endif
