/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_utils_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 03:57:29 by haito             #+#    #+#             */
/*   Updated: 2025/02/13 05:22:49 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "checker_bonus.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
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
		if (*str == '-')
			is_nega *= -1;
		str++;
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

long	ft_atol(const char *str)
{
	int				is_nega;
	unsigned long	result;

	result = 0;
	is_nega = 1;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			is_nega *= -1;
		str++;
	}
	while (ft_isdigit(*str))
	{
		result = result * 10 + *str++ - '0';
		if (result > LONG_MAX)
		{
			if (is_nega == -1)
				return (LONG_MIN);
			else
				return (LONG_MAX);
		}
	}
	return (result * is_nega);
}

int	check_sorted(t_stacks stacks, int sizeof_args)
{
	int	is_sorted;
	int	is_not_sorted;
	int	i;

	is_sorted = 1;
	is_not_sorted = 0;
	i = 0;
	if (sizeof_args != stacks.size_a)
		return (is_not_sorted);
	while (i < (sizeof_args - 1))
	{
		if (stacks.a[i].num > stacks.a[i + 1].num)
			return (is_not_sorted);
		i++;
	}
	return (is_sorted);
}
