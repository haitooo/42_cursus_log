/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 02:01:03 by haito             #+#    #+#             */
/*   Updated: 2025/04/06 22:36:42 by haito            ###   ########.fr       */
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
	while (my_is_digit(*str))
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

// long	get_time_in_ms(void)
// {
// 	struct timeval	tv;

// 	gettimeofday(&tv, NULL);
// 	printf("%ld\n", tv.tv_sec);
// 	printf("%ld\n", tv.tv_usec);
// 	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
// }
