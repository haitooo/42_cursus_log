/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 14:22:43 by haito             #+#    #+#             */
/*   Updated: 2024/11/25 18:18:53 by haito            ###   ########.fr       */
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

int	get_digit_ul(unsigned long num)
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
