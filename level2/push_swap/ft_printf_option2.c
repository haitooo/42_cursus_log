/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_option2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 18:11:28 by haito             #+#    #+#             */
/*   Updated: 2024/11/23 19:07:12 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	case_u(va_list args)
{
	unsigned int	num;

	num = va_arg(args, unsigned int);
	return (ft_putunbr(num));
}

int	case_x(va_list args)
{
	unsigned char	*hex;
	unsigned int	num;
	unsigned char	num_hex[9];
	int				i;

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
	return (ft_putstr((const char *)&num_hex[i]));
}

int	case_large_x(va_list args)
{
	unsigned char	*hex;
	unsigned int	num;
	unsigned char	num_hex[9];
	int				i;

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
	return (ft_putstr((const char *)&num_hex[i]));
}
