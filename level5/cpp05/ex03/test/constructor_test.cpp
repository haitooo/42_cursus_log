#include "Bureaucrat.hpp"
#include "AForm.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"
#include "Intern.hpp"
#include "TextFormatter.h"
#include <fstream>
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

static std::string	toString(AForm const & aform)
{
	std::ostringstream	oss;

	oss << aform;

	return (oss.str());
}

static bool	fileExists(std::string const & path)
{
	std::ifstream	ifs(path.c_str());

	return (ifs.is_open());
}

static void	testDerivedConstructors()
{
	section("derived forms: name and grades");

	ShrubberyCreationForm	shrubbery("home");
	RobotomyRequestForm		robotomy("Bender");
	PresidentialPardonForm	pardon("Arthur");

	check("ShrubberyCreationForm name", shrubbery.getName() == "ShrubberyCreationForm");
	check("ShrubberyCreationForm grades are 145 / 137", shrubbery.getGradeToSign() == 145 && shrubbery.getGradeToExec() == 137);
	check("RobotomyRequestForm name", robotomy.getName() == "RobotomyRequestForm");
	check("RobotomyRequestForm grades are 72 / 45", robotomy.getGradeToSign() == 72 && robotomy.getGradeToExec() == 45);
	check("PresidentialPardonForm name", pardon.getName() == "PresidentialPardonForm");
	check("PresidentialPardonForm grades are 25 / 5", pardon.getGradeToSign() == 25 && pardon.getGradeToExec() == 5);
	check("a new form starts unsigned", shrubbery.getIsSigned() == false);
}

static void	testDerivedDefaultConstructors()
{
	section("derived forms: default constructor");

	ShrubberyCreationForm	shrubbery;
	RobotomyRequestForm		robotomy;
	PresidentialPardonForm	pardon;

	check("ShrubberyCreationForm keeps its own name", shrubbery.getName() == "ShrubberyCreationForm");
	check("RobotomyRequestForm keeps its own grades", robotomy.getGradeToSign() == 72 && robotomy.getGradeToExec() == 45);
	check("PresidentialPardonForm keeps its own grades", pardon.getGradeToSign() == 25 && pardon.getGradeToExec() == 5);
}

static void	testDerivedCopyConstructor()
{
	section("derived forms: copy constructor");

	ShrubberyCreationForm	original("copyctor");
	Bureaucrat				boss("Boss", 1);

	original.beSigned(boss);

	ShrubberyCreationForm	copy(original);

	check("base part is copied: name", copy.getName() == "ShrubberyCreationForm");
	check("base part is copied: grades", copy.getGradeToSign() == 145 && copy.getGradeToExec() == 137);
	check("base part is copied: isSigned", copy.getIsSigned() == true);

	copy.execute(boss);
	check("derived part is copied: target", fileExists("copyctor_shrubbery"));
}

static void	testDerivedAssignment()
{
	section("derived forms: copy assignment");

	ShrubberyCreationForm	source("assign");
	ShrubberyCreationForm	target("other");
	Bureaucrat				boss("Boss", 1);

	source.beSigned(boss);
	target = source;

	check("isSigned is assigned", target.getIsSigned() == true);
	check("name stays unchanged (const member)", target.getName() == "ShrubberyCreationForm");

	target.execute(boss);
	check("target is assigned", fileExists("assign_shrubbery"));
}

static void	testExecuteRejectsUnsigned()
{
	section("AForm::execute rejects an unsigned form");

	PresidentialPardonForm	pardon("Arthur");
	Bureaucrat				boss("Boss", 1);
	bool					thrown;

	thrown = false;
	try { pardon.execute(boss); }
	catch (AForm::FormNotSignedException &) { thrown = true; }
	check("throws FormNotSignedException", thrown);

	thrown = false;
	try { pardon.execute(boss); }
	catch (std::exception &) { thrown = true; }
	check("exception is catchable as std::exception", thrown);
}

static void	testExecuteRejectsLowGrade()
{
	section("AForm::execute checks the executor grade");

	PresidentialPardonForm	pardon("Arthur");
	Bureaucrat				signer("Signer", 25);
	Bureaucrat				exact("Exact", 5);
	bool					thrown;

	pardon.beSigned(signer);
	check("grade 25 can sign it", pardon.getIsSigned() == true);

	thrown = false;
	try { pardon.execute(signer); }
	catch (AForm::GradeTooLowException &) { thrown = true; }
	check("grade 25 cannot execute it", thrown);

	thrown = false;
	try { pardon.execute(exact); }
	catch (std::exception &) { thrown = true; }
	check("grade 5 executes it without throwing", thrown == false);
}

static void	testPolymorphicExecute()
{
	section("polymorphism through AForm*");

	AForm*		forms[3];
	Bureaucrat	boss("Boss", 1);
	bool		thrown;
	int			i;

	forms[0] = new ShrubberyCreationForm("poly");
	forms[1] = new RobotomyRequestForm("Bender");
	forms[2] = new PresidentialPardonForm("Arthur");

	thrown = false;
	i = 0;
	while (i < 3)
	{
		try
		{
			boss.signForm(*forms[i]);
			forms[i]->execute(boss);
		}
		catch (std::exception &) { thrown = true; }
		i++;
	}
	check("every derived form signs and executes", thrown == false);
	check("ShrubberyCreationForm::action ran", fileExists("poly_shrubbery"));

	i = 0;
	while (i < 3)
	{
		delete forms[i];
		i++;
	}
	check("deleting through AForm* is safe (virtual destructor)", true);
}

static void	testAFormOutputOperator()
{
	section("AForm: operator<<");

	PresidentialPardonForm	pardon("Arthur");

	check("unsigned form reports \"is not signed\"", toString(pardon).find("PresidentialPardonForm is not signed.") == 0);
	check("output shows gradeToSign", toString(pardon).find("require grade to sign : 25") != std::string::npos);
	check("output shows gradeToExec", toString(pardon).find("require grade to execute : 5") != std::string::npos);
}

static void	runAFormTests()
{
	testDerivedConstructors();
	testDerivedDefaultConstructors();
	testDerivedCopyConstructor();
	testDerivedAssignment();
	testExecuteRejectsUnsigned();
	testExecuteRejectsLowGrade();
	testPolymorphicExecute();
	testAFormOutputOperator();
}


static void	testInternMakesEachForm()
{
	section("Intern: makeForm builds every catalogued form");

	Intern	intern;
	AForm*	shrubbery = intern.makeForm("shrubbery creation", "home");
	AForm*	robotomy = intern.makeForm("robotomy request", "Bender");
	AForm*	pardon = intern.makeForm("presidential pardon", "Arthur");

	check("\"shrubbery creation\" builds a ShrubberyCreationForm", shrubbery->getName() == "ShrubberyCreationForm");
	check("its grades are 145 / 137", shrubbery->getGradeToSign() == 145 && shrubbery->getGradeToExec() == 137);
	check("\"robotomy request\" builds a RobotomyRequestForm", robotomy->getName() == "RobotomyRequestForm");
	check("its grades are 72 / 45", robotomy->getGradeToSign() == 72 && robotomy->getGradeToExec() == 45);
	check("\"presidential pardon\" builds a PresidentialPardonForm", pardon->getName() == "PresidentialPardonForm");
	check("its grades are 25 / 5", pardon->getGradeToSign() == 25 && pardon->getGradeToExec() == 5);
	check("a new form starts unsigned", shrubbery->getIsSigned() == false);

	delete shrubbery;
	delete robotomy;
	delete pardon;
}

static void	testInternPassesTarget()
{
	section("Intern: the target reaches the form");

	Intern		intern;
	Bureaucrat	boss("Boss", 1);
	AForm*		form = intern.makeForm("shrubbery creation", "interntarget");

	boss.signForm(*form);
	form->execute(boss);
	check("the form was built with the given target", fileExists("interntarget_shrubbery"));

	delete form;
}

static void	testInternRejectsUnknownName()
{
	section("Intern: unknown names are rejected");

	Intern	intern;
	bool	thrown;

	thrown = false;
	try
	{
		AForm*	form = intern.makeForm("coffee break", "X");
		delete form;
	}
	catch (Intern::FormNotFoundException &) { thrown = true; }
	check("an unknown name throws FormNotFoundException", thrown);

	thrown = false;
	try
	{
		AForm*	form = intern.makeForm("Robotomy Request", "X");
		delete form;
	}
	catch (std::exception &) { thrown = true; }
	check("the lookup is case sensitive", thrown);

	thrown = false;
	try
	{
		AForm*	form = intern.makeForm("", "X");
		delete form;
	}
	catch (std::exception &) { thrown = true; }
	check("an empty name throws", thrown);
}

static void	testInternErrorMessage()
{
	section("Intern: the error message is explicit");

	Intern		intern;
	std::string	message;

	try
	{
		AForm*	form = intern.makeForm("coffee break", "X");
		delete form;
	}
	catch (std::exception & e) { message = e.what(); }

	check("the message repeats the rejected name", message.find("coffee break") != std::string::npos);
	check("the message lists shrubbery creation", message.find("shrubbery creation") != std::string::npos);
	check("the message lists robotomy request", message.find("robotomy request") != std::string::npos);
	check("the message lists presidential pardon", message.find("presidential pardon") != std::string::npos);
}

static void	testInternUsableFormNames()
{
	section("Intern: usableFormNames");

	std::string	names = Intern::usableFormNames();

	check("it lists shrubbery creation", names.find("shrubbery creation") != std::string::npos);
	check("it lists robotomy request", names.find("robotomy request") != std::string::npos);
	check("it lists presidential pardon", names.find("presidential pardon") != std::string::npos);
	check("the entries are separated", names.find(", ") != std::string::npos);
}

static void	testInternCanonicalForm()
{
	section("Intern: orthodox canonical form");

	Intern	original;
	Intern	copy(original);
	Intern	assigned;

	assigned = original;

	AForm*	fromCopy = copy.makeForm("presidential pardon", "Arthur");
	AForm*	fromAssigned = assigned.makeForm("robotomy request", "Bender");

	check("a copied Intern still works", fromCopy->getName() == "PresidentialPardonForm");
	check("an assigned Intern still works", fromAssigned->getName() == "RobotomyRequestForm");
	check("each call returns a distinct object", fromCopy != fromAssigned);

	delete fromCopy;
	delete fromAssigned;
}

static void	runInternTests()
{
	testInternMakesEachForm();
	testInternPassesTarget();
	testInternRejectsUnknownName();
	testInternErrorMessage();
	testInternUsableFormNames();
	testInternCanonicalForm();
}

static void	runAllTests()
{
	runBureaucratTests();
	runAFormTests();
	runInternTests();
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
