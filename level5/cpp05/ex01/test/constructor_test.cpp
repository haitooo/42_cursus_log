#include "Bureaucrat.hpp"
#include "Form.hpp"
#include "TextFormatter.h"
#include <iostream>
#include <sstream>
#include <string>

static int	g_pass = 0;
static int	g_fail = 0;

static void	check(std::string const & label, bool ok)
{
	if (ok)
	{
		std::cout << GREEN << "  ✓ " << RESET << label << std::endl;
		g_pass++;
	}
	else
	{
		std::cout << RED << "  × " << RESET << label << std::endl;
		g_fail++;
	}
}

static void	section(std::string const & title)
{
	std::cout << std::endl << YELLOW << title << RESET << std::endl;
}

static std::string	toString(Bureaucrat const & bureaucrat)
{
	std::ostringstream	oss;

	oss << bureaucrat;

	return (oss.str());
}

static void	testBureaucratDefaultConstructor()
{
	section("Bureaucrat: default constructor");

	Bureaucrat	bureaucrat;

	check("name is \"Bob\"", bureaucrat.getName() == "Bob");
	check("grade is 150", bureaucrat.getGrade() == 150);
}

static void	testBureaucratParamConstructor()
{
	section("Bureaucrat: parameterized constructor");

	Bureaucrat	bureaucrat("Alice", 42);
	Bureaucrat	highest("Top", 1);
	Bureaucrat	lowest("Bottom", 150);

	check("name is kept", bureaucrat.getName() == "Alice");
	check("grade is kept", bureaucrat.getGrade() == 42);
	check("grade 1 is accepted", highest.getGrade() == 1);
	check("grade 150 is accepted", lowest.getGrade() == 150);
}

static void	testBureaucratConstructorThrows()
{
	section("Bureaucrat: constructor rejects out of range");

	bool	thrown;

	thrown = false;
	try
	{
		Bureaucrat	tmp("X", 0);
		(void)tmp;
	}
	catch (Bureaucrat::GradeTooHighException &) { thrown = true; }
	check("grade 0 throws GradeTooHighException", thrown);

	thrown = false;
	try
	{
		Bureaucrat	tmp("X", 151);
		(void)tmp;
	}
	catch (Bureaucrat::GradeTooLowException &) { thrown = true; }
	check("grade 151 throws GradeTooLowException", thrown);

	thrown = false;
	try
	{
		Bureaucrat	tmp("X", -1);
		(void)tmp;
	}
	catch (Bureaucrat::GradeTooHighException &) { thrown = true; }
	check("grade -1 throws GradeTooHighException", thrown);

	thrown = false;
	try
	{
		Bureaucrat	tmp("X", 0);
		(void)tmp;
	}
	catch (std::exception &) { thrown = true; }
	check("exception is catchable as std::exception", thrown);
}

static void	testBureaucratCopyConstructor()
{
	section("Bureaucrat: copy constructor");

	Bureaucrat	original("Alice", 42);
	Bureaucrat	copy(original);

	check("name is copied", copy.getName() == "Alice");
	check("grade is copied", copy.getGrade() == 42);

	copy.incrementGrade();
	check("copy does not share state with original", original.getGrade() == 42 && copy.getGrade() == 41);
}

static void	testBureaucratAssignment()
{
	section("Bureaucrat: copy assignment");

	Bureaucrat	source("Alice", 42);
	Bureaucrat	target("Bob", 100);
	Bureaucrat&	alias = source;

	target = source;
	check("grade is assigned", target.getGrade() == 42);
	check("name stays unchanged (const member)", target.getName() == "Bob");

	source = alias;
	check("self assignment keeps the value", source.getGrade() == 42 && source.getName() == "Alice");
}

static void	testBureaucratIncrementDecrement()
{
	section("Bureaucrat: incrementGrade / decrementGrade");

	Bureaucrat	bureaucrat("Alice", 100);
	Bureaucrat	top("Top", 1);
	Bureaucrat	bottom("Bottom", 150);
	bool		thrown;

	bureaucrat.incrementGrade();
	check("incrementGrade lowers the number", bureaucrat.getGrade() == 99);

	bureaucrat.decrementGrade();
	check("decrementGrade raises the number", bureaucrat.getGrade() == 100);

	thrown = false;
	try { top.incrementGrade(); }
	catch (Bureaucrat::GradeTooHighException &) { thrown = true; }
	check("incrementGrade at grade 1 throws", thrown);
	check("grade is unchanged after the throw", top.getGrade() == 1);

	thrown = false;
	try { bottom.decrementGrade(); }
	catch (Bureaucrat::GradeTooLowException &) { thrown = true; }
	check("decrementGrade at grade 150 throws", thrown);
	check("grade is unchanged after the throw", bottom.getGrade() == 150);
}

static void	testBureaucratOutputOperator()
{
	section("Bureaucrat: operator<<");

	Bureaucrat	bureaucrat("Alice", 42);

	check("format matches the subject", toString(bureaucrat) == "Alice, bureaucrat grade 42.");
}

static void	runBureaucratTests()
{
	testBureaucratDefaultConstructor();
	testBureaucratParamConstructor();
	testBureaucratConstructorThrows();
	testBureaucratCopyConstructor();
	testBureaucratAssignment();
	testBureaucratIncrementDecrement();
	testBureaucratOutputOperator();
}

static std::string	toString(Form const & form)
{
	std::ostringstream	oss;

	oss << form;

	return (oss.str());
}

static void	testFormDefaultConstructor()
{
	section("Form: default constructor");

	Form	form;

	check("name is \"DefaultForm\"", form.getName() == "DefaultForm");
	check("isSigned is false", form.getIsSigned() == false);
	check("gradeToSign is 150", form.getGradeToSign() == 150);
	check("gradeToExec is 150", form.getGradeToExec() == 150);
}

static void	testFormParamConstructor()
{
	section("Form: parameterized constructor");

	Form	form("Contract", 40, 30);
	Form	edge("Edge", 1, 150);

	check("name is kept", form.getName() == "Contract");
	check("gradeToSign is kept", form.getGradeToSign() == 40);
	check("gradeToExec is kept", form.getGradeToExec() == 30);
	check("a new form starts unsigned", form.getIsSigned() == false);
	check("grade 1 and 150 are accepted", edge.getGradeToSign() == 1 && edge.getGradeToExec() == 150);
}

static void	testFormConstructorThrows()
{
	section("Form: constructor rejects out of range");

	bool	thrown;

	thrown = false;
	try
	{
		Form	tmp("X", 0, 30);
		(void)tmp;
	}
	catch (Form::GradeTooHighException &) { thrown = true; }
	check("gradeToSign 0 throws GradeTooHighException", thrown);

	thrown = false;
	try
	{
		Form	tmp("X", 40, 151);
		(void)tmp;
	}
	catch (Form::GradeTooLowException &) { thrown = true; }
	check("gradeToExec 151 throws GradeTooLowException", thrown);

	thrown = false;
	try
	{
		Form	tmp("X", 0, 30);
		(void)tmp;
	}
	catch (std::exception &) { thrown = true; }
	check("exception is catchable as std::exception", thrown);
}

static void	testFormCopyConstructor()
{
	section("Form: copy constructor");

	Form		original("Contract", 40, 30);
	Bureaucrat	signer("Signer", 10);

	original.beSigned(signer);

	Form	copy(original);

	check("name is copied", copy.getName() == "Contract");
	check("gradeToSign is copied", copy.getGradeToSign() == 40);
	check("gradeToExec is copied", copy.getGradeToExec() == 30);
	check("isSigned is copied", copy.getIsSigned() == true);
}

static void	testFormAssignment()
{
	section("Form: copy assignment");

	Form		source("Source", 40, 30);
	Form		target("Target", 100, 90);
	Bureaucrat	signer("Signer", 10);

	source.beSigned(signer);
	target = source;

	check("isSigned is assigned", target.getIsSigned() == true);
	check("name stays unchanged (const member)", target.getName() == "Target");
	check("gradeToSign stays unchanged (const member)", target.getGradeToSign() == 100);
}

static void	testFormBeSigned()
{
	section("Form: beSigned");

	Form		form("Contract", 40, 30);
	Form		exact("Exact", 40, 30);
	Form		refused("Refused", 40, 30);
	Bureaucrat	high("High", 10);
	Bureaucrat	same("Same", 40);
	Bureaucrat	low("Low", 41);
	bool		thrown;

	form.beSigned(high);
	check("a higher grade signs the form", form.getIsSigned() == true);

	exact.beSigned(same);
	check("an equal grade signs the form", exact.getIsSigned() == true);

	thrown = false;
	try { refused.beSigned(low); }
	catch (Form::GradeTooLowException &) { thrown = true; }
	check("a lower grade throws GradeTooLowException", thrown);
	check("the form stays unsigned after the throw", refused.getIsSigned() == false);
}

static void	testFormOutputOperator()
{
	section("Form: operator<<");

	Form	form("Contract", 40, 30);

	check("unsigned form reports \"is not signed\"", toString(form).find("Contract is not signed.") == 0);
	check("output shows gradeToSign", toString(form).find("require grade to sign : 40") != std::string::npos);
	check("output shows gradeToExec", toString(form).find("require grade to execute : 30") != std::string::npos);
}

static void	runFormTests()
{
	testFormDefaultConstructor();
	testFormParamConstructor();
	testFormConstructorThrows();
	testFormCopyConstructor();
	testFormAssignment();
	testFormBeSigned();
	testFormOutputOperator();
}

static void	runAllTests()
{
	runBureaucratTests();
	runFormTests();
}

int	main(void)
{
	runAllTests();

	std::cout << std::endl;
	if (g_fail == 0)
		std::cout << GREEN << "all passed: " << g_pass << " / " << (g_pass + g_fail) << RESET << std::endl;
	else
		std::cout << RED << "failed: " << g_fail << " / " << (g_pass + g_fail) << RESET << std::endl;

	return (g_fail == 0 ? 0 : 1);
}
