/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_option.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 14:42:31 by haito             #+#    #+#             */
/*   Updated: 2024/11/25 04:39:46 by haito            ###   ########.fr       */
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

int	case_s(va_list args, t_flag *flag)
{
	const char	*str;
	int			result;
	int			length;

	result = 0;
	str = va_arg(args, const char *);
	if (str == NULL)
		return (ft_putstr("(null)"));
	length = ft_strlen(str);
	while (result < flag->flag_right - length && flag->flag_left == 0)
	{
		if (ft_putchar(' ') == -1)
			return (-1);
		result++;
	}
	if (ft_putstr(str) == -1)
		return (-1);
	result += length;
	while (result < flag->flag_left)
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
		return (ft_putstr("(nil)"));
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
	return (ft_print_addr(addr_hex, flag) + i);
}

int	case_d_i_zero(va_list args, t_flag *flag)
{
	int	num;
	int	result;
	int	result_put;
	int	digit_num;

	result = 0;
	num = va_arg(args, int);
	digit_num = get_digit_zero(num);
	if (num < 0)
	{
		if (ft_putchar('-') == -1)
			return (-1);
		num = -num;
		result++;
	}
	while (result < flag->flag_zero - digit_num)
	{
		if (ft_putchar('0') == -1)
			return (-1);
		result++;
	}
	if (num)
	{
		result_put = ft_putnbr(num, flag);
		if (result_put == -1)
			return (-1);
		result += result_put;
	}
	return (result);
}

int	case_d_i(va_list args, t_flag *flag)
{
	int	num;
	int	result;
	int	result_put;
	int	digit_num;

	result = 0;
	num = va_arg(args, int);
	digit_num = get_digit(num);
	while (result < flag->flag_right - digit_num && flag->flag_left == 0)
	{
		if (ft_putchar(' ') == -1)
			return (-1);
		result++;
	}
	result_put = ft_putnbr(num, flag);
	if (result_put == -1)
		return (-1);
	result += result_put;
	while (result < flag->flag_left)
	{
		if (ft_putchar(' ') == -1)
			return (-1);
		result++;
	}
	return (result);
}
