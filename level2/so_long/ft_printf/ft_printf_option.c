/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_option.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 14:42:31 by haito             #+#    #+#             */
/*   Updated: 2024/11/23 19:21:00 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	case_c(va_list args)
{
	int	c;

	c = va_arg(args, int);
	return (ft_putchar(c));
}

int	case_s(va_list args)
{
	const char	*str;
	int			result;

	result = 0;
	str = va_arg(args, const char *);
	if (str == NULL)
		return (ft_putstr("(null)"));
	while (*str)
	{
		if (ft_putchar(*(str++)) == -1)
			return (-1);
		result += 1;
	}
	return (result);
}

int	case_p(va_list args)
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
	return (ft_print_addr(addr_hex));
}

int	case_d_i(va_list args)
{
	int	num;

	num = va_arg(args, int);
	return (ft_putnbr(num));
}

int	case_ld(va_list args)
{
	long	num;

	num = va_arg(args, long);
	return (ft_putnbr_long(num));
}
