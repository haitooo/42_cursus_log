/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 18:38:21 by haito             #+#    #+#             */
/*   Updated: 2024/11/25 03:51:21 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_atoi(const char *str)
{
	int				is_nega;
	unsigned long	result;

	result = 0;
	is_nega = 1;
	while ((*str >= 9 && *str <= 13) || *str == ' ')
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			is_nega *= -1;
		str++;
	}
	while (ft_isdigit(*str))
	{
		result = result * 10 + *str++ - '0';
		if (result > LONG_MAX)
		{
			if (is_nega == -1)
				return ((int)LONG_MIN);
			else
				return ((int)LONG_MAX);
		}
	}
	return ((int)(result * is_nega));
}

int	ft_putstr(const char *s)
{
	int	result;

	result = 0;
	while (*s)
	{
		if (write(1, s++, 1) == -1)
			return (-1);
		result += 1;
	}
	return (result);
}

int	ft_putchar(char c)
{
	return (write(1, &c, 1));
}

int	ft_putunbr(unsigned int n)
{
	char	c;
	int		result;

	result = 0;
	if (n > 9)
		result += ft_putunbr(n / 10);
	c = (n % 10) + '0';
	if (ft_putchar(c) == -1)
		return (-1);
	result += 1;
	return (result);
}

int	ft_putnbr(int n, t_flag *flag)
{
	char	c;
	int		result;

	result = 0;
	if (n == -2147483648)
	{
		if (flag->flag_zero == 0)
			return (ft_putstr("-2147483648"));
		else
			return (ft_putstr("2147483648"));
	}
	if (n < 0)
	{
		if (ft_putchar('-') == -1)
			return (-1);
		result += 1;
		n = -n;
	}
	if (n > 9)
		result += ft_putnbr(n / 10, flag);
	c = (n % 10) + '0';
	if (ft_putchar(c) == -1)
		return (-1);
	result += 1;
	return (result);
}
