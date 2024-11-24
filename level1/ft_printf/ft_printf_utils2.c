/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 18:38:21 by haito             #+#    #+#             */
/*   Updated: 2024/11/25 05:05:56 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

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

int	ft_strlen(const char *s)
{
	int	count;

	count = 0;
	while (s[count])
		count++;
	return (count);
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

int	ft_putnbr(int n)
{
	char	c;
	int		result;

	result = 0;
	if (n == -2147483648)
		return (ft_putstr("-2147483648"));
	if (n < 0)
	{
		if (ft_putchar('-') == -1)
			return (-1);
		result += 1;
		n = -n;
	}
	if (n > 9)
		result += ft_putnbr(n / 10);
	c = (n % 10) + '0';
	if (ft_putchar(c) == -1)
		return (-1);
	result += 1;
	return (result);
}
