/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_option3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 03:00:21 by haito             #+#    #+#             */
/*   Updated: 2024/11/25 03:22:45 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	help_case_x(const char *str, t_flag *flag, char c)
{
	int	result;
	int	result_put;
	int	length;

	result = 0;
	length = ft_strlen(str);
	while ((result < flag->flag_right - length && flag->flag_left == 0)
		|| result < flag->flag_zero - length)
	{
		if (ft_putchar(c) == -1)
			return (-1);
		result++;
	}
	result_put = ft_putstr(str);
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

int	case_x(va_list args, t_flag *flag)
{
	unsigned char	*hex;
	unsigned int	num;
	unsigned char	num_hex[9];
	int				i;
	int				result;

	i = 8;
	num = va_arg(args, unsigned int);
	num_hex[8] = '\0';
	if (num == 0)
		return (ft_putchar('0'));
	hex = (unsigned char *)"0123456789abcdef";
	while (num != 0)
	{
		num_hex[--i] = hex[num % 16];
		num /= 16;
	}
	if (flag->flag_zero == 0)
		result = help_case_x((const char *)&num_hex[i], flag, ' ');
	else
		result = help_case_x((const char *)&num_hex[i], flag, '0');
	return (result);
}

int	case_large_x(va_list args, t_flag *flag)
{
	unsigned char	*hex;
	unsigned int	num;
	unsigned char	num_hex[9];
	int				i;
	int				result;


	i = 8;
	num = va_arg(args, unsigned int);
	num_hex[8] = '\0';
	if (num == 0)
		return (ft_putchar('0'));
	hex = (unsigned char *)"0123456789ABCDEF";
	while (num != 0)
	{
		num_hex[--i] = hex[num % 16];
		num /= 16;
	}
	if (flag->flag_zero == 0)
		result = help_case_x((const char *)&num_hex[i], flag, ' ');
	else
		result = help_case_x((const char *)&num_hex[i], flag, '0');
	return (result);
}
