#include "Bureaucrat.hpp"
#include "TextFormatter.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

static std::string	prompt(std::string const & message)
{
	std::string	line;

	std::cout << CYAN << message << RESET << std::endl;
	if (!std::getline(std::cin, line))
		throw std::runtime_error("input closed");

	return (line);
}

static int	parseGrade(std::string const & str)
{
	std::istringstream	iss(str);
	std::string			rest;
	int					value;

	if (!(iss >> value) || (iss >> rest))
		throw std::runtime_error("grade must be an integer");

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

	return (Bureaucrat(name, parseGrade(grade_str)));
}

static void	runCommandLoop(Bureaucrat & bureaucrat)
{
	std::string	command;

	while (1)
	{
		std::cout << YELLOW << "plz select command. [i] increment / [d] decrement / [q] quit" << RESET << std::endl;
		if (!std::getline(std::cin, command) || command == "q")
			break ;

		try
		{
			if (command == "i")
				bureaucrat.incrementGrade();
			else if (command == "d")
				bureaucrat.decrementGrade();
			else
			{
				std::cout << MAGENTA << "unknown command: " << command << RESET << std::endl;
				continue ;
			}
			std::cout << bureaucrat << std::endl;
		}
		catch (std::exception & e)
		{
			std::cout << RED << e.what() << RESET << std::endl;
		}
	}
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
