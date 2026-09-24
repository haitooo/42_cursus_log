#ifndef INTERN_HPP
# define INTERN_HPP

#include <exception>
#include <string>

#include "AForm.hpp"

class	Intern {
	private:
		typedef AForm*	(*FormMaker)(std::string const & target);

		struct	FormEntry {
			char const *	name;
			FormMaker		maker;
		};

		static FormEntry const	formTable[];
		static int const		formCount;

		static AForm*	makeShrubberyCreationForm(std::string const & target);
		static AForm*	makeRobotomyRequestForm(std::string const & target);
		static AForm*	makePresidentialPardonForm(std::string const & target);

	public:
		Intern();
		Intern(const Intern& other);
		Intern&	operator=(const Intern& other);
		~Intern();

		AForm*	makeForm(std::string const & formName, std::string const & target) const;

		static std::string	usableFormNames();

		class	FormNotFoundException : public std::exception {
			private:
				std::string	message;

			public:
				FormNotFoundException(std::string const & formName);
				virtual ~FormNotFoundException() throw();

				virtual const char*	what() const throw();
		};
};

#endif
