#ifndef FORM_HPP
# define FORM_HPP

#include <iostream>
#include <string>
#include <exception>
#include "Bureaucrat.hpp"

class	Form {
	private:
		std::string const	name;
		bool				isSigned;
		int	const			gradeToSign;
		int	const			gradeToExec;
		static int const	highestGrade;
		static int const	lowestGrade;

		static int	validGrade(int value);

	public:
		Form();
		Form(std::string const & name, int const gradeToSign, int const gradeToExec);
		Form(const Form& other);
		Form&	operator=(const Form& other);
		~Form();

		std::string const &	getName() const;
		bool				getIsSigned() const;
		int				getGradeToSign() const;
		int				getGradeToExec() const;

		void	beSigned(const Bureaucrat& bureaucrat);

		class	GradeTooHighException : public std::exception {
			public:
				virtual const char*	what() const throw();
		};
		class	GradeTooLowException : public std::exception {
			public:
				virtual const char*	what() const throw();
		};
};

std::ostream&	operator<<(std::ostream& os, const Form& form);

#endif
