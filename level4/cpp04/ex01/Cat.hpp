#ifndef CAT_HPP
# define CAT_HPP

#include "Animal.hpp"
#include "Brain.hpp"
#include "TextFormatter.h"

class	Cat : public Animal
{
	private:
		Brain*	brain;
	public:
		Cat();
		Cat(const Cat& other);
		Cat& operator=(const Cat& other);
		virtual ~Cat();

		Brain&	getBrain() const;

		void	makeSound() const; /*override*/
};

#endif
