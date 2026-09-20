#include "Bureaucrat.hpp"
#include "Form.hpp"
#include "TextFormatter.h"
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

static void	createForm(Form** forms, int & count)
{
	if (count >= MAX_FORM)
	{
		std::cout << MAGENTA << "form list is full" << RESET << std::endl;
		return ;
	}

	std::string	name = prompt("Enter Form Name");
	int			grade_to_sign = parseInt(prompt("Enter Grade To Sign"));
	int			grade_to_exec = parseInt(prompt("Enter Grade To Execute"));

	forms[count] = new Form(name, grade_to_sign, grade_to_exec);
	count++;
	std::cout << GREEN << "form created" << RESET << std::endl;
}

static void	showFormList(Form** forms, int count)
{
	if (count == 0)
	{
		std::cout << MAGENTA << "no form yet" << RESET << std::endl;
		return ;
	}

	for (int i = 0; i < count; i++)
		std::cout << "[" << i << "] " << *forms[i] << std::endl;
}

static void	selectAndSign(Bureaucrat const & bureaucrat, Form** forms, int count)
{
	int	index;

	if (count == 0)
	{
		std::cout << MAGENTA << "no form yet" << RESET << std::endl;
		return ;
	}

	showFormList(forms, count);
	index = parseInt(prompt("Enter Form Number"));
	if (index < 0 || index >= count)
	{
		std::cout << MAGENTA << "no such form: " << index << RESET << std::endl;
		return ;
	}

	bureaucrat.signForm(*forms[index]);
}

static void	clearForms(Form** forms, int & count)
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
	Form*		forms[MAX_FORM];
	int			count = 0;
	std::string	command;

	for (int i = 0; i < MAX_FORM; i++)
		forms[i] = NULL;

	while (1)
	{
		std::cout << YELLOW << "plz select command. [i] increment / [d] decrement / [m] make form / [f] form list / [s] sign form / [q] quit" << RESET << std::endl;
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
