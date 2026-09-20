#include "Bureaucrat.hpp"
#include "AForm.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"
#include "TextFormatter.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#define MAX_FORM 10

static std::string	prompt(std::string const & message)
{
	std::string	line;

	std::cout << CYAN << message << RESET << std::endl;
	if (!std::getline(std::cin, line))
		throw std::runtime_error("input closed");

	return (line);
}

static int	parseInt(std::string const & str)
{
	std::istringstream	iss(str);
	std::string			rest;
	int					value;

	if (!(iss >> value) || (iss >> rest))
		throw std::runtime_error("not an integer");

	return (value);
}

static Bureaucrat	createBureaucrat()
{
	std::string	name = prompt("Enter Name");
	std::string	grade_str = prompt("Enter Grade");

	if (grade_str.empty())
	{
		std::cout << RED << "Grade is empty. created with default Name \"Bob\" and lowestgrade 150" << RESET << std::endl;
		return (Bureaucrat());
	}

	return (Bureaucrat(name, parseInt(grade_str)));
}

static AForm*	makeForm(std::string const & type, std::string const & target)
{
	if (type == "1")
		return (new ShrubberyCreationForm(target));
	if (type == "2")
		return (new RobotomyRequestForm(target));
	if (type == "3")
		return (new PresidentialPardonForm(target));

	return (NULL);
}

static void	createForm(AForm** forms, int & count)
{
	if (count >= MAX_FORM)
	{
		std::cout << MAGENTA << "form list is full" << RESET << std::endl;
		return ;
	}

	std::string	type = prompt("Select Form Type. [1] shrubbery creation / [2] robotomy request / [3] presidential pardon");
	std::string	target = prompt("Enter Target");
	AForm*		form = makeForm(type, target);

	if (form == NULL)
	{
		std::cout << MAGENTA << "no such form type: " << type << RESET << std::endl;
		return ;
	}

	forms[count] = form;
	count++;
	std::cout << GREEN << "form created" << RESET << std::endl;
}

static void	showFormList(AForm** forms, int count)
{
	if (count == 0)
	{
		std::cout << MAGENTA << "no form yet" << RESET << std::endl;
		return ;
	}

	for (int i = 0; i < count; i++)
		std::cout << "[" << i << "] " << *forms[i] << std::endl;
}

static int	selectFormIndex(AForm** forms, int count)
{
	int	index;

	if (count == 0)
	{
		std::cout << MAGENTA << "no form yet" << RESET << std::endl;
		return (-1);
	}

	showFormList(forms, count);
	index = parseInt(prompt("Enter Form Number"));
	if (index < 0 || index >= count)
	{
		std::cout << MAGENTA << "no such form: " << index << RESET << std::endl;
		return (-1);
	}

	return (index);
}

static void	selectAndSign(Bureaucrat const & bureaucrat, AForm** forms, int count)
{
	int	index = selectFormIndex(forms, count);

	if (index >= 0)
		bureaucrat.signForm(*forms[index]);
}

static void	selectAndExecute(Bureaucrat const & bureaucrat, AForm** forms, int count)
{
	int	index = selectFormIndex(forms, count);

	if (index >= 0)
		bureaucrat.executeForm(*forms[index]);
}

static void	clearForms(AForm** forms, int & count)
{
	for (int i = 0; i < count; i++)
	{
		delete forms[i];
		forms[i] = NULL;
	}
	count = 0;
}

static void	changeGrade(Bureaucrat & bureaucrat, std::string const & command)
{
	int	count = parseInt(prompt("Enter How Many Grades"));

	if (count <= 0)
	{
		std::cout << MAGENTA << "count must be positive: " << count << RESET << std::endl;
		return ;
	}

	try
	{
		while (count > 0)
		{
			if (command == "i")
				bureaucrat.incrementGrade();
			else
				bureaucrat.decrementGrade();
			count--;
		}
	}
	catch (std::exception & e)
	{
		std::cout << RED << e.what() << RESET << std::endl;
	}
	std::cout << bureaucrat << std::endl;
}

static void	runCommandLoop(Bureaucrat & bureaucrat)
{
	AForm*		forms[MAX_FORM];
	int			count = 0;
	std::string	command;

	for (int i = 0; i < MAX_FORM; i++)
		forms[i] = NULL;

	while (1)
	{
		std::cout << YELLOW << "plz select command. [i] increment / [d] decrement / [m] make form / [f] form list / [s] sign form / [e] execute form / [q] quit" << RESET << std::endl;
		if (!std::getline(std::cin, command) || command == "q")
			break ;

		try
		{
			if (command == "i" || command == "d")
				changeGrade(bureaucrat, command);
			else if (command == "m")
				createForm(forms, count);
			else if (command == "f")
				showFormList(forms, count);
			else if (command == "s")
				selectAndSign(bureaucrat, forms, count);
			else if (command == "e")
				selectAndExecute(bureaucrat, forms, count);
			else
				std::cout << MAGENTA << "unknown command: " << command << RESET << std::endl;
		}
		catch (std::exception & e)
		{
			std::cout << RED << e.what() << RESET << std::endl;
		}
	}

	clearForms(forms, count);
}

int	main(void)
{
	std::srand(std::time(NULL));

	try
	{
		Bureaucrat	bureaucrat = createBureaucrat();

		std::cout << GREEN << "succes create bureaucrat!!" << RESET << std::endl;
		std::cout << bureaucrat << std::endl;

		runCommandLoop(bureaucrat);
	}
	catch (std::exception & e)
	{
		std::cout << RED << e.what() << RESET << std::endl;
		return (1);
	}

	return (0);
}
