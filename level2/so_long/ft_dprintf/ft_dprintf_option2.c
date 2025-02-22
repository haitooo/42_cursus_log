/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fprintf_option2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 18:00:02 by haito             #+#    #+#             */
/*   Updated: 2025/02/22 01:07:24 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"

int	fcase_u(va_list args)
{
	unsigned int	num;

	num = va_arg(args, unsigned int);
	return (ft_fputunbr(num));
}

int	fcase_x(va_list args)
{
	unsigned char	*hex;
	unsigned int	num;
	unsigned char	num_hex[9];
	int				i;

	i = 8;
	num = va_arg(args, unsigned int);
	num_hex[8] = '\0';
	if (num == 0)
		return (ft_fputchar('0'));
	hex = (unsigned char *)"0123456789abcdef";
	while (num != 0)
	{
		num_hex[--i] = hex[num % 16];
		num /= 16;
	}
	return (ft_fputstr((const char *)&num_hex[i]));
}

int	fcase_large_x(va_list args)
{
	unsigned char	*hex;
	unsigned int	num;
	unsigned char	num_hex[9];
	int				i;

	i = 8;
	num = va_arg(args, unsigned int);
	num_hex[8] = '\0';
	if (num == 0)
		return (ft_fputchar('0'));
	hex = (unsigned char *)"0123456789ABCDEF";
	while (num != 0)
	{
		num_hex[--i] = hex[num % 16];
		num /= 16;
	}
	return (ft_fputstr((const char *)&num_hex[i]));
}
