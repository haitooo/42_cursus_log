#ifndef ICE_HPP
# define ICE_HPP

#include <iostream>
#include "AMateria.hpp"

class	Ice : public AMateria
{
	public:
		Ice();
		Ice(const Ice& other);
		Ice&	operator=(const Ice& other);
		virtual ~Ice();

		AMateria*	clone() const; /*override*/
		void		use(ICharacter& target); /*override*/
};

#endif
