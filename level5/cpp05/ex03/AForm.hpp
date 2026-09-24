#ifndef AFORM_HPP
# define AFORM_HPP

#include <iostream>
#include <string>
#include <exception>
#include "Bureaucrat.hpp"

class	AForm {
	private:
		std::string const	name;
		bool				isSigned;
		int	const			gradeToSign;
		int	const			gradeToExec;
		static int const	highestGrade;
		static int const	lowestGrade;

		static int	validGrade(int value);

	protected:
		virtual void	action() const = 0;
	
	public:
		AForm();
		AForm(std::string const & name, int const gradeToSign, int const gradeToExec);
		AForm(const AForm& other);
		AForm&	operator=(const AForm& other);
		virtual ~AForm();

		std::string const &	getName() const;
		bool				getIsSigned() const;
		int				getGradeToSign() const;
		int				getGradeToExec() const;

		void	beSigned(const Bureaucrat& bureaucrat);
		void	execute(Bureaucrat const & executor) const;

		class	GradeTooHighException : public std::exception {
			public:
				virtual const char*	what() const throw();
		};
		class	GradeTooLowException : public std::exception {
			public:
				virtual const char*	what() const throw();
		};
		class	FormNotSignedException : public std::exception {
			public:
				virtual const char*	what() const throw();
		};
};

std::ostream&	operator<<(std::ostream& os, const AForm& aform);

#endif
