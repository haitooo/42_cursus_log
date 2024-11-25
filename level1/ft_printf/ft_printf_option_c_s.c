/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_option_c_s.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 14:42:31 by haito             #+#    #+#             */
/*   Updated: 2024/11/25 21:09:41 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	case_c(va_list args, t_flag *flag)
{
	int	c;
	int	result;

	result = 0;
	c = va_arg(args, int);
	while (result < flag->flag_right - 1 && flag->flag_left == 0)
	{
		if (ft_putchar(' ') == -1)
			return (-1);
		result++;
	}
	if (ft_putchar(c) == -1)
		return (-1);
	result++;
	while (result < flag->flag_left)
	{
		if (ft_putchar(' ') == -1)
			return (-1);
		result++;
	}
	return (result);
}

int	help_case_s(t_flag *flag, int result, int total_size, const char *str)
{
	while (result < flag->flag_right - total_size && flag->flag_left == 0)
	{
		if (ft_putchar(' ') == -1)
			return (-1);
		result++;
	}
	if (ft_putstr_s(str, total_size) == -1)
		return (-1);
	result += total_size;
	while (result < flag->flag_left)
	{
		if (ft_putchar(' ') == -1)
			return (-1);
		result++;
	}
	return (result);
}

int	case_s(va_list args, t_flag *flag)
{
	const char	*str;
	int			result;
	int			result_tmp;
	int			length;
	int			total_size;

	result = 0;
	str = va_arg(args, const char *);
	if (str == NULL)
	{
		if (!(flag->flag_period < 6) || flag->flag_right != 0
			|| (flag->flag_period == -1 && flag->flag_right == 0))
		{
			while (result++ < flag->flag_right - 6)
				if (ft_putchar(' ') == -1)
					return (-1);
			return (ft_putstr("(null)") + result - 1);
		}
		return (0);
	}
	length = ft_strlen(str);
	if (flag->flag_period >= 0 && flag->flag_period < length)
		total_size = flag->flag_period;
	else
		total_size = length;
	result_tmp = help_case_s(flag, result, total_size, str);
	if (result_tmp == -1)
		return (-1);
	return (result + result_tmp);
}

int	help_case_p(t_flag *flag)
{
	int	result;

	result = 0;
	while (result < flag->flag_right - 5)
	{
		if (ft_putchar(' ') == -1)
			return (-1);
		result++;
	}
	return (result);
}

int	case_p(va_list args, t_flag *flag)
{
	void			*addr;
	unsigned char	*hex;
	unsigned char	addr_hex[17];
	unsigned long	address;
	int				i;

	addr = va_arg(args, void *);
	address = (unsigned long)addr;
	if (address == 0)
		return (help_case_p(flag) + ft_putstr("(nil)"));
	hex = (unsigned char *)"0123456789abcdef";
	i = 15;
	while (i >= 0)
	{
		addr_hex[i--] = hex[address % 16];
		address /= 16;
	}
	i = 0;
	while (i < flag->flag_right - 14 && flag->flag_left == 0)
	{
		if (ft_putchar(' ') == -1)
			return (-1);
		i++;
	}
	return (ft_print_addr(addr_hex, flag, -1) + i);
}
