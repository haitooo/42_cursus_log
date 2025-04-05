/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 02:01:03 by haito             #+#    #+#             */
/*   Updated: 2025/04/04 11:03:34 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	my_is_digit(int c)
{
	return (c >= '0' && c <= '9');
}

int	ft_atoi(const char *str)
{
	int				is_nega;
	unsigned long	result;

	result = 0;
	is_nega = 1;
	while ((*str >= 9 && *str <= 13) || *str == ' ')
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str++ == '-')
			is_nega *= -1;
	}
	while (ft_isdigit(*str))
	{
		result = result * 10 + *str++ - '0';
		if (result > LONG_MAX)
		{
			if (is_nega == -1)
				return ((int)LONG_MIN);
			else
				return ((int)LONG_MAX);
		}
	}
	return ((int)(result * is_nega));
}
