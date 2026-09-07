#include "Bureaucrat.hpp"
#include "TextFormatter.h"

/*
** Adjacent string literals are concatenated by the preprocessor, so these tags
** cost nothing at runtime: BOLD GREEN "[ ok ]" RESET is a single literal.
*/
#define TAG_OK		BOLD GREEN "[ ok  ]" RESET
#define TAG_THROW	BOLD CYAN "[throw]" RESET
#define TAG_BAD		BOLD RED "[ !!  ]" RESET
#define TAG_NOTE	DIM "        ->" RESET

static void	title(int n, std::string const & text)
{
	std::cout << std::endl
		<< BOLD << YELLOW << "==== " << n << ". " << text << " ====" << RESET << std::endl;
}

int	main(void)
{
	title(1, "valid grades, including both bounds");
	try
	{
		Bureaucrat	alice("Alice", 42);
		Bureaucrat	best("Best", 1);
		Bureaucrat	worst("Worst", 150);

		std::cout << TAG_OK << " " << alice << std::endl;
		std::cout << TAG_OK << " " << best << std::endl;
		std::cout << TAG_OK << " " << worst << std::endl;
	}
	catch (std::exception & e)
	{
		std::cout << TAG_BAD << " unexpected: " << e.what() << std::endl;
	}

	title(2, "construction with grade 0 -> too high");
	try
	{
		Bureaucrat	nope("Nope", 0);
		std::cout << TAG_BAD << " " << nope << std::endl;
	}
	catch (Bureaucrat::GradeTooHighException & e)
	{
		std::cout << TAG_THROW << " caught by exact type: " << BWHITE << e.what() << RESET << std::endl;
	}
	catch (std::exception & e)
	{
		std::cout << TAG_BAD << " caught by base: " << e.what() << std::endl;
	}

	title(3, "construction with grade 151 -> too low");
	try
	{
		Bureaucrat	nope("Nope", 151);
		std::cout << TAG_BAD << " " << nope << std::endl;
	}
	catch (Bureaucrat::GradeTooLowException & e)
	{
		std::cout << TAG_THROW << " caught by exact type: " << BWHITE << e.what() << RESET << std::endl;
	}
	catch (std::exception & e)
	{
		std::cout << TAG_BAD << " caught by base: " << e.what() << std::endl;
	}

	title(4, "increment lowers the number, decrement raises it");
	try
	{
		Bureaucrat	bob("Bob", 3);

		std::cout << TAG_OK << " start:                    " << bob << std::endl;
		bob.incrementGrade();
		std::cout << TAG_OK << " after incrementGrade():   " << bob << std::endl;
		bob.decrementGrade();
		bob.decrementGrade();
		std::cout << TAG_OK << " after 2x decrementGrade(): " << bob << std::endl;
	}
	catch (std::exception & e)
	{
		std::cout << TAG_BAD << " unexpected: " << e.what() << std::endl;
	}

	title(5, "incrementGrade() at grade 1 -> too high");
	try
	{
		Bureaucrat	top("Top", 1);

		std::cout << TAG_OK << " " << top << std::endl;
		top.incrementGrade();
		std::cout << TAG_BAD << " NOT REACHED" << std::endl;
	}
	catch (std::exception & e)
	{
		std::cout << TAG_THROW << " caught: " << BWHITE << e.what() << RESET << std::endl;
	}

	title(6, "decrementGrade() at grade 150 -> too low");
	try
	{
		Bureaucrat	bottom("Bottom", 150);

		std::cout << TAG_OK << " " << bottom << std::endl;
		bottom.decrementGrade();
		std::cout << TAG_BAD << " NOT REACHED" << std::endl;
	}
	catch (std::exception & e)
	{
		std::cout << TAG_THROW << " caught: " << BWHITE << e.what() << RESET << std::endl;
	}

	title(7, "the object survives a thrown exception unchanged");
	{
		Bureaucrat	carol("Carol", 1);

		try
		{
			carol.incrementGrade();
		}
		catch (std::exception & e)
		{
			std::cout << TAG_THROW << " caught: " << BWHITE << e.what() << RESET << std::endl;
		}
		std::cout << TAG_OK << " still usable: " << carol << std::endl;
	}

	title(8, "copy constructor and copy assignment");
	{
		Bureaucrat	original("Original", 10);
		Bureaucrat	copied(original);
		Bureaucrat	assigned("Assigned", 100);

		assigned = original;
		std::cout << TAG_OK << " original: " << original << std::endl;
		std::cout << TAG_OK << " copied:   " << copied << std::endl;
		std::cout << TAG_OK << " assigned: " << assigned << std::endl;
		std::cout << TAG_NOTE << " the name is const, so assignment only copies the grade" << std::endl;
	}

	title(9, "default constructor");
	{
		Bureaucrat	def;

		std::cout << TAG_OK << " " << def << std::endl;
	}

	std::cout << std::endl;

	return (0);
}
