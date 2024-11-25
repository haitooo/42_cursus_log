/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_option_x.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 03:00:21 by haito             #+#    #+#             */
/*   Updated: 2024/11/25 21:37:46 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	help_case_x(const char *str, t_flag *flag, char c, int result)
{
	int	result_put;
	int	size;

	size = ft_strlen(str);
	while ((result < flag->flag_right - size && flag->flag_left == 0)
		|| result < flag->flag_zero - size || result < flag->flag_period - size)
	{
		if (ft_putchar(c) == -1)
			return (-1);
		result++;
	}
	if (flag->flag_period != 0 || (flag->flag_period == 0 && *str != '0'))
	{
		result_put = ft_putstr(str);
		if (result_put == -1)
			return (-1);
		result += result_put;
	}
	while (result < flag->flag_left)
	{
		if (ft_putchar(' ') == -1)
			return (-1);
		result++;
	}
	return (result);
}

int	flagcheck_case_x(t_flag *flag, const char *str)
{
	int	result;

	result = 0;
	if (flag->flag_hash && *str != '0')
	{
		if (ft_putstr("0x") == -1)
			return (-1);
		result += 2;
	}
	if (flag->flag_zero == 0 && flag->flag_period == -1)
		result += help_case_x(str, flag, ' ', 0);
	else
		result += help_case_x(str, flag, '0', 0);
	return (result);
}

int	flagcheck_case_large_x(t_flag *flag, const char *str)
{
	int	result;

	result = 0;
	if (flag->flag_hash && *str != '0')
	{
		if (ft_putstr("0X") == -1)
			return (-1);
		result += 2;
	}
	if (flag->flag_zero == 0 && flag->flag_period == -1)
		result += help_case_x(str, flag, ' ', 0);
	else
		result += help_case_x(str, flag, '0', 0);
	return (result);
}

int	case_x(va_list args, t_flag *flag)
{
	unsigned char	*hex;
	unsigned long	num;
	unsigned char	num_hex[20];
	int				i;
	int				result;

	result = 19;
	num = va_arg(args, unsigned long);
	num = (unsigned int)num;
	i = 19;
	num_hex[i] = '\0';
	if (num == 0)
		num_hex[--i] = '0';
	hex = (unsigned char *)"0123456789abcdef";
	while (i > 0 && num != 0)
	{
		num_hex[--i] = hex[num % 16];
		num /= 16;
	}
	return (flagcheck_case_x(flag, (const char *)&num_hex[i]));
}

int	case_large_x(va_list args, t_flag *flag)
{
	unsigned char	*hex;
	unsigned int	num;
	unsigned char	num_hex[20];
	int				i;
	int				result;

	result = 0;
	num = va_arg(args, unsigned int);
	i = 0;
	while (i < 19)
	{
		num_hex[i] = 0;
		i++;
	}
	num_hex[i] = '\0';
	if (num == 0)
		num_hex[--i] = '0';
	hex = (unsigned char *)"0123456789ABCDEF";
	while (num != 0)
	{
		num_hex[--i] = hex[num % 16];
		num /= 16;
	}
	return (flagcheck_case_large_x(flag, (const char *)&num_hex[i]));
}
