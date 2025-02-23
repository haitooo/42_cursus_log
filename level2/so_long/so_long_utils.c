/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 01:01:54 by haito             #+#    #+#             */
/*   Updated: 2025/02/22 08:19:12 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

size_t	ft_strlen(const char *s)
{
	int	count;

	count = 0;
	while (s && s[count])
		count++;
	return (count);
}

int	ft_int_strlen(const char *s)
{
	int	count;

	count = 0;
	while (s && s[count])
		count++;
	return (count);
}

int	my_power(int base, int exp)
{
	int	result;

	result = 1;
	while (exp > 0)
	{
		result *= base;
		exp--;
	}
	return (result);
}
