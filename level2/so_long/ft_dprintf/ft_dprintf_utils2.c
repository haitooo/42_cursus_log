/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fprintf_utils2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 18:00:08 by haito             #+#    #+#             */
/*   Updated: 2025/02/22 01:07:24 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"

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
