/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_option_d_i.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 13:01:28 by haito             #+#    #+#             */
/*   Updated: 2024/11/25 21:56:08 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	help_case_d_i(int num, t_flag *flag, int *result)
{
	while ((flag->flag_right && flag->flag_period != -1
			&& *result < flag->flag_right - flag->flag_period)
		&& *result < flag->flag_left - flag->flag_period)
	{
		if (ft_putchar(' ') == -1)
			return (-1);
		(*result)++;
	}
	// while (*result < flag->flag_period - flag->flag_left)
	// {
	// 	if (ft_putchar('0') == -1)
	// 		return (-1);
	// 	(*result)++;
	// }
	if (flag->flag_plus && num >= 0)
	{
		if (ft_putchar('+') == -1)
			return (-1);
		(*result)++;
	}
	else if (flag->flag_space && num >= 0)
	{
		if (ft_putchar(' ') == -1)
			return (-1);
		(*result)++;
	}
	return (0);
}

int	help_case_d_i_p_z(int result, t_flag *flag, int digit_num, int num)
{
	int	result_put;

	while (flag->flag_zero && flag->flag_period != -1
		&& result < flag->flag_zero - flag->flag_period)
	{
		if (ft_putchar(' ') == -1)
			return (-1);
		result++;
	}
	while (result < flag->flag_zero - digit_num
		|| result < flag->flag_period - digit_num
		|| result < flag->flag_right - digit_num)
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

int	case_d_i(va_list args, t_flag *flag, int result)
{
	int	num;
	int	result_put;
	int	digit_num;

	num = va_arg(args, int);
	digit_num = get_digit(num);
	if (flag->flag_period == -1 && help_case_d_i(num, flag, &result) == -1)
		return (-1);
	while (result < flag->flag_right - digit_num && flag->flag_left == 0 && num)
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

int	case_d_i_zero(va_list args, t_flag *flag, int result)
{
	int	num;
	int	result_put;
	int	digit_num;

	num = va_arg(args, int);
	digit_num = get_digit_zero(num);
	if (help_case_d_i(num, flag, &result) == -1)
		return (-1);
	if (num < 0)
	{
		if (ft_putchar('-') == -1)
			return (-1);
		num = -num;
		result++;
	}
	result_put = help_case_d_i_p_z(result, flag, digit_num, num);
	if (result_put == -1)
		return (-1);
	return (result_put);
}

int	case_d_i_period(va_list args, t_flag *flag, int result)
{
	int	num;
	int	digit_num;

	num = va_arg(args, int);
	digit_num = get_digit_zero(num);
	if (help_case_d_i(num, flag, &result) == -1)
		return (-1);
	if (num < 0)
	{
		if (num == INT_MIN)
		{
			if (ft_putchar('-') == -1)
				return (-1);
			while (result < flag->flag_period - 10)
			{
				if (ft_putchar('0') == -1)
					return (0);
				result++;
			}
			if (ft_putchar('2') == -1)
				return (-1);
			num = -147483648;
			result++;
		}
		else if (ft_putchar('-') == -1)
			return (-1);
		num = -num;
		result++;
		flag->flag_period++;
	}
	return (help_case_d_i_p_z(result, flag, digit_num, num));
}
