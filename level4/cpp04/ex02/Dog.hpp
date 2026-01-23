#ifndef DOG_HPP
# define DOG_HPP

#include "AAnimal.hpp"
#include "Brain.hpp"
#include "TextFormatter.h"

class	Dog : public AAnimal
{
	private:
		Brain*	brain;
	public:
		Dog();
		Dog(const Dog& other);
		Dog& operator=(const Dog& other);
		virtual ~Dog();

		Brain&	getBrain() const;

		void	makeSound() const; /*override*/
};

#endif
