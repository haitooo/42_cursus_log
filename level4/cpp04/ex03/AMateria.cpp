#include "AMateria.hpp"

AMateria::AMateria() : type("default")
{
}

AMateria::AMateria(std::string const & type) : type(type)
{
}

AMateria::AMateria(const AMateria& other) : type(other.type)
{
}

AMateria&	AMateria::operator=(const AMateria& other)
{
	// A Materia’s is an inherent property, so copying it does not make sense.
	(void)other;
	return (*this);
}

AMateria::~AMateria()
{
}

std::string const &	AMateria::getType() const
{
	return (type);
}

void	AMateria::use(ICharacter& target)
{
	(void)target;
}
