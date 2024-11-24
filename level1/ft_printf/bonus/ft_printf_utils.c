/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 14:22:43 by haito             #+#    #+#             */
/*   Updated: 2024/11/25 04:33:11 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	get_digit(int num)
{
	int	count;

	count = 0;
	if (num < 0)
		count++;
	while (num)
	{
		num /= 10;
		count++;
	}
	return (count);
}

int	get_digit_zero(int num)
{
	int	count;

	count = 0;
	while (num)
	{
		num /= 10;
		count++;
	}
	return (count);
}

int	get_digit_u(unsigned int num)
{
	int	count;

	count = 0;
	while (num)
	{
		num /= 10;
		count++;
	}
	return (count);
}

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

size_t	ft_strlen(const char *s)
{
	int	count;

	count = 0;
	while (s[count])
		count++;
	return (count);
}

int	ft_print_addr(unsigned char *addr_hex, t_flag *flag)
{
	int		i;
	int		result;
	int		has_num;

	result = 2;
	has_num = 0;
	if (ft_putstr("0x") == -1)
		return (-1);
	i = -1;
	while (i++ < 15)
	{
		if (addr_hex[i] != '0' || has_num)
		{
			has_num = 1;
			if (ft_putchar(addr_hex[i]) == -1)
				return (-1);
			result++;
		}
	}
	while (result < flag->flag_left)
	{
		if (ft_putchar(' ') == -1)
			return (-1);
		result++;
	}
	return (result);
}
