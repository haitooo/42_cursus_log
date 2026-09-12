#include "Bureaucrat.hpp"
#include "Form.hpp"

int const	Form::highestGrade = 1;
int const	Form::lowestGrade = 150;

Form::Form() : name("DefaultForm"), isSigned(false), gradeToSign(lowestGrade), gradeToExec(lowestGrade)
{
}

Form::Form(std::string const & name, int const gradeToSign, int const gradeToExec) : name(name), isSigned(false), gradeToSign(validGrade(gradeToSign)), gradeToExec(validGrade(gradeToExec))
{
}

Form::Form(const Form& other) : name(other.name), isSigned(other.isSigned), gradeToSign(other.gradeToSign), gradeToExec(other.gradeToExec)
{
}

Form&	Form::operator=(const Form& other)
{
	if (this != &other)
		isSigned = other.isSigned;

	return (*this);
}

Form::~Form()
{
}

std::string const &	Form::getName() const
{
	return (name);
}

bool	Form::getIsSigned() const
{
	return (isSigned);
}

int	Form::getGradeToSign() const
{
	return (gradeToSign);
}

int	Form::getGradeToExec() const
{
	return (gradeToExec);
}

void	Form::beSigned(const Bureaucrat& bureaucrat)
{
	int	grade = bureaucrat.getGrade();

	if (grade <= gradeToSign)
	{
		isSigned = true;
		return ;
	}
	throw GradeTooLowException();
}

int	Form::validGrade(int value)
{
	if (value < highestGrade)
		throw GradeTooHighException();
	if (value > lowestGrade)
		throw GradeTooLowException();

	return (value);
}

const char*	Form::GradeTooHighException::what() const throw()
{
	return ("Form: grade is too high");
}

const char*	Form::GradeTooLowException::what() const throw()
{
	return ("Form: grade is too low");
}

std::ostream&	operator<<(std::ostream& os, const Form& form)
{
	os << form.getName() << " is " << (form.getIsSigned() ? "signed" : "not signed") << ".\n"
		<< "require grade to sign : " << form.getGradeToSign() << "\n"
		<< "require grade to execute : " << form.getGradeToExec();

	return (os);
}
