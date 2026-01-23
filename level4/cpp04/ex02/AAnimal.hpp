#ifndef AANIMAL_HPP
# define AANIMAL_HPP

#include <iostream>
#include <string>
#include "TextFormatter.h"

class	AAnimal
{
	protected:
		std::string	type;
	public:
		AAnimal();
		AAnimal(std::string type);
		AAnimal(const AAnimal& other);
		AAnimal& operator=(const AAnimal& other);
		virtual ~AAnimal();

		const std::string&	getType() const;
		void				setType(const std::string& type);

		virtual void	makeSound() const = 0;
};

#endif
