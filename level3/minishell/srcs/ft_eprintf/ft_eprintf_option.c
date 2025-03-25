/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_eprintf_option.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 17:59:59 by haito             #+#    #+#             */
/*   Updated: 2025/03/22 18:41:15 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_eprintf.h"

int	ecase_c(va_list args)
{
	int	c;

	c = va_arg(args, int);
	return (ft_eputchar(c));
}

int	ecase_s(va_list args)
{
	const char	*str;
	int			result;

	result = 0;
	str = va_arg(args, const char *);
	if (str == NULL)
		return (ft_eputstr("(null)"));
	while (*str)
	{
		if (ft_eputchar(*(str++)) == -1)
			return (-1);
		result += 1;
	}
	return (result);
}

int	ecase_p(va_list args)
{
	void			*addr;
	unsigned char	*hex;
	unsigned char	addr_hex[17];
	unsigned long	address;
	int				i;

	addr = va_arg(args, void *);
	address = (unsigned long)addr;
	if (address == 0)
		return (ft_eputstr("(nil)"));
	hex = (unsigned char *)"0123456789abcdef";
	i = 15;
	while (i >= 0)
	{
		addr_hex[i--] = hex[address % 16];
		address /= 16;
	}
	return (ft_eprint_addr(addr_hex));
}

int	ecase_d_i(va_list args)
{
	int	num;

	num = va_arg(args, int);
	return (ft_eputnbr(num));
}

int	ecase_ld(va_list args)
{
	long	num;

	num = va_arg(args, long);
	return (ft_eputnbr_long(num));
}
