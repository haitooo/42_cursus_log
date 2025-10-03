#include "Account.hpp"
#include <iostream>
#include <ctime>

int	Account::_nbAccounts = 0;
int	Account::_totalAmount = 0;
int	Account::_totalNbDeposits = 0;
int	Account::_totalNbWithdrawals = 0;

Account::Account(int initial_deposit)
{
	_accountIndex = Account::_nbAccounts;
	_amount = initial_deposit;
	_nbDeposits = 0;
	_nbWithdrawals = 0;

	Account::_nbAccounts += 1;
	Account::_totalAmount += initial_deposit;

	_displayTimestamp();
	std::cout	<< " index:" << _accountIndex
				<< ";amount:" << checkAmount()
				<< ";created" << std::endl;
}

Account::~Account(void)
{
	_displayTimestamp();
	std::cout	<< " index:" << _accountIndex
				<< ";amount:" << checkAmount()
				<< ";closed" << std::endl;
}

int	Account::checkAmount(void) const
{
	return (_amount);
}

int	Account::getNbAccounts(void)
{
	return (Account::_nbAccounts);
}

int	Account::getTotalAmount(void)
{
	return (Account::_totalAmount);
}

int	Account::getNbDeposits(void)
{
	return (Account::_totalNbDeposits);
}

int	Account::getNbWithdrawals(void)
{
	return (Account::_totalNbWithdrawals);
}

void	Account::_displayTimestamp(void)
{
	std::time_t	now = std::time(NULL);
	std::tm		*ltm = std::localtime(&now);

	char	buffer[20];
	std::strftime(buffer, sizeof(buffer), "[%Y%m%d_%H%M%S]", ltm);

	std::cout << buffer;
}

void	Account::displayAccountsInfos(void)
{
	_displayTimestamp();
	std::cout	<< " accounts:" << Account::getNbAccounts()
				<< ";total:" << Account::getTotalAmount()
				<< ";deposits:" << Account::getNbDeposits()
				<< ";withdrawals:" << Account::getNbWithdrawals() << std::endl;
}

void	Account::displayStatus(void) const
{
	_displayTimestamp();
	std::cout	<< " index:" << _accountIndex
				<< ";amount:" << checkAmount()
				<< ";deposits:" << _nbDeposits
				<< ";withdrawals:" << _nbWithdrawals << std::endl;
}

void	Account::makeDeposit(int deposit)
{
	int	tmp_amount = checkAmount();

	_amount += deposit;
	Account::_totalAmount += deposit;
	_nbDeposits += 1;
	Account::_totalNbDeposits += 1;

	_displayTimestamp();
	std::cout	<< " index:" << _accountIndex
				<< ";p_amount:" << tmp_amount
				<< ";deposit:" << deposit
				<< ";amount:" << checkAmount()
				<< ";nb_deposits:" << _nbDeposits << std::endl;
}

bool	Account::makeWithdrawal(int withdrawal)
{
	int	tmp_amount = checkAmount();

	if (tmp_amount < withdrawal)
	{
		_displayTimestamp();
		std::cout	<< " index:" << _accountIndex
					<< ";p_amount:" << tmp_amount
					<< ";withdrawal:refused" << std::endl;
		return (false);
	}
	_amount -= withdrawal;
	Account::_totalAmount -= withdrawal;
	_nbWithdrawals += 1;
	Account::_totalNbWithdrawals += 1;

	_displayTimestamp();
	std::cout	<< " index:" << _accountIndex
				<< ";p_amount:" << tmp_amount
				<< ";withdrawal:" << withdrawal
				<< ";amount:" << checkAmount()
				<< ";nb_withdrawals:" << _nbWithdrawals << std::endl;
	return (true);
}
