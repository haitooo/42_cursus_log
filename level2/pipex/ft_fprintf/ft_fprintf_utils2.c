#include "ft_fprintf.h"

int	ft_fputstr(const char *s)
{
	int	result;

	result = 0;
	while (*s)
	{
		if (write(2, s++, 1) == -1)
			return (-1);
		result += 1;
	}
	return (result);
}

int	ft_fputchar(char c)
{
	return (write(2, &c, 1));
}

int	ft_fputunbr(unsigned int n)
{
	char	c;
	int		result;

	result = 0;
	if (n > 9)
		result += ft_fputunbr(n / 10);
	c = (n % 10) + '0';
	if (ft_fputchar(c) == -1)
		return (-1);
	result += 1;
	return (result);
}

int	ft_fputnbr(int n)
{
	char	c;
	int		result;

	result = 0;
	if (n == -2147483648)
		return (ft_fputstr("-2147483648"));
	if (n < 0)
	{
		if (ft_fputchar('-') == -1)
			return (-1);
		result += 1;
		n = -n;
	}
	if (n > 9)
		result += ft_fputnbr(n / 10);
	c = (n % 10) + '0';
	if (ft_fputchar(c) == -1)
		return (-1);
	result += 1;
	return (result);
}

int	ft_fputnbr_long(long n)
{
	char	c;
	int		result;

	result = 0;
	if (n == LONG_MIN)
		return (ft_fputstr("-9223372036854775808"));
	if (n < 0)
	{
		if (ft_fputchar('-') == -1)
			return (-1);
		result += 1;
		n = -n;
	}
	if (n > 9)
		result += ft_fputnbr(n / 10);
	c = (n % 10) + '0';
	if (ft_fputchar(c) == -1)
		return (-1);
	result += 1;
	return (result);
}
