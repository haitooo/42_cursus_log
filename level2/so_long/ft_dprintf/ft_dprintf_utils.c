/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fprintf_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 18:00:04 by haito             #+#    #+#             */
/*   Updated: 2025/02/22 01:07:24 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_dprintf.h"

int	ft_fprint_addr(unsigned char *addr_hex)
{
	int		i;
	int		result;
	int		has_num;

	result = 0;
	has_num = 0;
	if (ft_fputstr("0x") == -1)
		return (-1);
	result += 2;
	i = -1;
	while (i++ < 15)
	{
		if (addr_hex[i] != '0' || has_num)
		{
			has_num = 1;
			if (ft_fputchar(addr_hex[i]) == -1)
				return (-1);
			result += 1;
		}
	}
	return (result);
}
