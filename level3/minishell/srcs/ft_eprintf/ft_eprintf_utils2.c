/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_eprintf_utils2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 18:00:08 by haito             #+#    #+#             */
/*   Updated: 2025/03/22 18:42:20 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_eprintf.h"

int	ft_eputstr(const char *s)
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

int	ft_eputchar(char c)
{
	return (write(2, &c, 1));
}

int	ft_eputunbr(unsigned int n)
{
	char	c;
	int		result;

	result = 0;
	if (n > 9)
		result += ft_eputunbr(n / 10);
	c = (n % 10) + '0';
	if (ft_eputchar(c) == -1)
		return (-1);
	result += 1;
	return (result);
}

int	ft_eputnbr(int n)
{
	char	c;
	int		result;

	result = 0;
	if (n == -2147483648)
		return (ft_eputstr("-2147483648"));
	if (n < 0)
	{
		if (ft_eputchar('-') == -1)
			return (-1);
		result += 1;
		n = -n;
	}
	if (n > 9)
		result += ft_eputnbr(n / 10);
	c = (n % 10) + '0';
	if (ft_eputchar(c) == -1)
		return (-1);
	result += 1;
	return (result);
}

int	ft_eputnbr_long(long n)
{
	char	c;
	int		result;

	result = 0;
	if (n == LONG_MIN)
		return (ft_eputstr("-9223372036854775808"));
	if (n < 0)
	{
		if (ft_eputchar('-') == -1)
			return (-1);
		result += 1;
		n = -n;
	}
	if (n > 9)
		result += ft_eputnbr(n / 10);
	c = (n % 10) + '0';
	if (ft_eputchar(c) == -1)
		return (-1);
	result += 1;
	return (result);
}
