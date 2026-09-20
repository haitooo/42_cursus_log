#include "Bureaucrat.hpp"
#include "AForm.hpp"

int const	AForm::highestGrade = 1;
int const	AForm::lowestGrade = 150;

AForm::AForm() : name("DefaultAForm"), isSigned(false), gradeToSign(lowestGrade), gradeToExec(lowestGrade)
{
}

AForm::AForm(std::string const & name, int const gradeToSign, int const gradeToExec) : name(name), isSigned(false), gradeToSign(validGrade(gradeToSign)), gradeToExec(validGrade(gradeToExec))
{
}

AForm::AForm(const AForm& other) : name(other.name), isSigned(other.isSigned), gradeToSign(other.gradeToSign), gradeToExec(other.gradeToExec)
{
}

AForm&	AForm::operator=(const AForm& other)
{
	if (this != &other)
		isSigned = other.isSigned;

	return (*this);
}

AForm::~AForm()
{
}

std::string const &	AForm::getName() const
{
	return (name);
}

bool	AForm::getIsSigned() const
{
	return (isSigned);
}

int	AForm::getGradeToSign() const
{
	return (gradeToSign);
}

int	AForm::getGradeToExec() const
{
	return (gradeToExec);
}

void	AForm::beSigned(const Bureaucrat& bureaucrat)
{
	int	grade = bureaucrat.getGrade();

	if (grade <= gradeToSign)
	{
		isSigned = true;
		return ;
	}
	throw GradeTooLowException();
}

void	AForm::execute(Bureaucrat const & executor) const
{
	if (!isSigned)
		throw FormNotSignedException();
	if (executor.getGrade() > gradeToExec)
		throw GradeTooLowException();

	action();
}

int	AForm::validGrade(int value)
{
	if (value < highestGrade)
		throw GradeTooHighException();
	if (value > lowestGrade)
		throw GradeTooLowException();

	return (value);
}

const char*	AForm::GradeTooHighException::what() const throw()
{
	return ("AForm: grade is too high");
}

const char*	AForm::GradeTooLowException::what() const throw()
{
	return ("AForm: grade is too low");
}

const char*	AForm::FormNotSignedException::what() const throw()
{
	return ("AForm: form is not signed");
}

std::ostream&	operator<<(std::ostream& os, const AForm& aform)
{
	os << aform.getName() << " is " << (aform.getIsSigned() ? "signed" : "not signed") << ".\n"
		<< "require grade to sign : " << aform.getGradeToSign() << "\n"
		<< "require grade to execute : " << aform.getGradeToExec();

	return (os);
}
