#ifndef MATERIASOURCE_HPP
# define MATERIASOURCE_HPP

#include "IMateriaSource.hpp"
#include "AMateria.hpp"

class MateriaSource : public IMateriaSource
{
	private:
		AMateria*	templates[4];
	public:
		MateriaSource();
		MateriaSource(const MateriaSource& other);
		MateriaSource&	operator=(const MateriaSource& other);
		virtual ~MateriaSource();

		void		learnMateria(AMateria* m); /*override*/
		AMateria*	createMateria(std::string const & type); /*override*/
		int			countEmptySlots() const;
};

#endif
