/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 14:22:43 by haito             #+#    #+#             */
/*   Updated: 2024/11/23 19:27:25 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_print_addr(unsigned char *addr_hex)
{
	int		i;
	int		result;
	int		has_num;

	result = 0;
	has_num = 0;
	if (ft_putstr("0x") == -1)
		return (-1);
	result += 2;
	i = -1;
	while (i++ < 15)
	{
		if (addr_hex[i] != '0' || has_num)
		{
			has_num = 1;
			if (ft_putchar(addr_hex[i]) == -1)
				return (-1);
			result += 1;
		}
	}
	return (result);
}
