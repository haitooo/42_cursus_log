#include "Bureaucrat.hpp"

int const	Bureaucrat::highestGrade = 1;
int const	Bureaucrat::lowestGrade = 150;

Bureaucrat::Bureaucrat() : name("Bob"), grade(lowestGrade)
{
}

Bureaucrat::Bureaucrat(std::string const & name, int grade) : name(name), grade(validGrade(grade))
{
}

Bureaucrat::Bureaucrat(const Bureaucrat& other) : name(other.name), grade(other.grade)
{
}

Bureaucrat&	Bureaucrat::operator=(const Bureaucrat& other)
{
	if (this != &other)
		grade = other.grade;

	return (*this);
}

Bureaucrat::~Bureaucrat()
{
}

std::string const &	Bureaucrat::getName() const
{
	return (name);
}

int	Bureaucrat::getGrade() const
{
	return (grade);
}

void	Bureaucrat::incrementGrade()
{
	grade = validGrade(grade - 1);
}

void	Bureaucrat::decrementGrade()
{
	grade = validGrade(grade + 1);
}

std::ostream&	operator<<(std::ostream& os, const Bureaucrat& bureaucrat)
{
	os << bureaucrat.getName() << ", bureaucrat grade " << bureaucrat.getGrade() << ".";

	return (os);
}

const char*	Bureaucrat::GradeTooHighException::what() const throw()
{
	return ("Bureaucrat: grade is too high");
}

const char*	Bureaucrat::GradeTooLowException::what() const throw()
{
	return ("Bureaucrat: grade is too low");
}

int	Bureaucrat::validGrade(int value)
{
	if (value < highestGrade)
		throw GradeTooHighException();
	if (value > lowestGrade)
		throw GradeTooLowException();

	return (value);
}
