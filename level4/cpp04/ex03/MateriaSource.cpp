#include "MateriaSource.hpp"

MateriaSource::MateriaSource()
{
	for (int i = 0; i < 4; i++)
		templates[i] = NULL;
}

MateriaSource::MateriaSource(const MateriaSource& other)
{
	for (int i = 0; i < 4; i++)
	{
		templates[i] = NULL;
		if (other.templates[i] != NULL)
			templates[i] = other.templates[i]->clone();
	}
}

MateriaSource&	MateriaSource::operator=(const MateriaSource& other)
{
	if (this == &other)
		return (*this);

	for (int i = 0; i < 4; i++)
	{
		if (templates[i] != NULL)
		{
			delete templates[i];
			templates[i] = NULL;
		}
		if (other.templates[i] != NULL)
			templates[i] = other.templates[i]->clone();
	}
	return (*this);
}

MateriaSource::~MateriaSource()
{
	for (int i = 0; i < 4; i++)
	{
		delete templates[i];
		templates[i] = NULL;
	}
}

void	MateriaSource::learnMateria(AMateria* m)
{
	if (m == NULL)
		return ;

	for (int i = 0; i < 4; i++)
	{
		if (templates[i] == NULL)
		{
			templates[i] = m->clone();
			delete m;
			return ;
		}
	}
	delete m;
}

AMateria*	MateriaSource::createMateria(std::string const & type)
{
	for (int i = 0; i < 4; i++)
	{
		if (templates[i] != NULL && templates[i]->getType() == type)
			return (templates[i]->clone());
	}
	return (0);
}

int			MateriaSource::countEmptySlots() const
{
	int	counter = 0;

	for (int i = 0; i < 4; i++)
	{
		if (templates[i] == NULL)
			counter++;
	}
	return (counter);
}
