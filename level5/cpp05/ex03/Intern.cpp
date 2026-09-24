#include "Intern.hpp"
#include "AForm.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"
#include <iostream>

Intern::FormEntry const	Intern::formTable[] = {
	{ "shrubbery creation", &Intern::makeShrubberyCreationForm },
	{ "robotomy request", &Intern::makeRobotomyRequestForm },
	{ "presidential pardon", &Intern::makePresidentialPardonForm }
};

int const	Intern::formCount = sizeof(formTable) / sizeof(formTable[0]);

Intern::Intern()
{
}

Intern::Intern(const Intern& other)
{
	(void)other;
}

Intern&	Intern::operator=(const Intern& other)
{
	(void)other;

	return (*this);
}

Intern::~Intern()
{
}

AForm*	Intern::makeShrubberyCreationForm(std::string const & target)
{
	return (new ShrubberyCreationForm(target));
}

AForm*	Intern::makeRobotomyRequestForm(std::string const & target)
{
	return (new RobotomyRequestForm(target));
}

AForm*	Intern::makePresidentialPardonForm(std::string const & target)
{
	return (new PresidentialPardonForm(target));
}

AForm*	Intern::makeForm(std::string const & formName, std::string const & target) const
{
	for (int i = 0; i < formCount; i++)
	{
		if (formName == formTable[i].name)
		{
			std::cout << "Intern creates " << formName << std::endl;
			return (formTable[i].maker(target));
		}
	}

	throw FormNotFoundException(formName);
}

std::string	Intern::usableFormNames()
{
	std::string	names;

	for (int i = 0; i < formCount; i++)
	{
		if (i > 0)
			names += ", ";
		names += formTable[i].name;
	}

	return (names);
}

Intern::FormNotFoundException::FormNotFoundException(std::string const & formName)
	: message("Intern: no form named \"" + formName + "\". usable: " + usableFormNames())
{
}

Intern::FormNotFoundException::~FormNotFoundException() throw()
{
}

const char*	Intern::FormNotFoundException::what() const throw()
{
	return (message.c_str());
}
