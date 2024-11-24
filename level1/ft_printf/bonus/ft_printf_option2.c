/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_option2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 18:11:28 by haito             #+#    #+#             */
/*   Updated: 2024/11/25 04:54:01 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	help_case_u(unsigned int num, t_flag *flag)
{
	int	result;
	int	result_put;

	result = 0;
	if (num || !flag->flag_zero)
	{
		result_put = ft_putunbr(num);
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

int	case_u(va_list args, t_flag *flag)
{
	unsigned int	num;
	int				result;
	int				result_put;
	int				digit_num;
	char			c;

	result = 0;
	if (flag->flag_zero == 0)
		c = ' ';
	else
		c = '0';
	num = va_arg(args, unsigned int);
	digit_num = get_digit_u(num);
	while ((result < flag->flag_right - digit_num && flag->flag_left == 0)
		|| result < flag->flag_zero - digit_num)
	{
		if (ft_putchar(c) == -1)
			return (-1);
		result++;
	}
	result_put = help_case_u(num, flag);
	if (result_put == -1)
		return (-1);
	return (result + result_put);
}
