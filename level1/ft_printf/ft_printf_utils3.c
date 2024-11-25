/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 12:48:04 by haito             #+#    #+#             */
/*   Updated: 2024/11/25 15:54:19 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_print_addr(unsigned char *addr_hex, t_flag *flag, int i)
{
	int		result;
	int		has_num;

	result = 2;
	has_num = 0;
	if (ft_putstr("0x") == -1)
		return (-1);
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

int	ft_strlen(const char *s)
{
	int	count;

	count = 0;
	while (s[count])
		count++;
	return (count);
}
