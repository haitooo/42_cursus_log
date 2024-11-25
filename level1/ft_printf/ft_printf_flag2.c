/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_flag2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 01:52:21 by haito             #+#    #+#             */
/*   Updated: 2024/11/25 19:44:39 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

const char	*check_flag_hash(t_flag *flag, const char *str)
{
	flag->flag_hash = 1;
	str++;
	flag->flag_bytes_flag++;
	return (str);
}

const char	*check_flag_plus(t_flag *flag, const char *str)
{
	flag->flag_plus = 1;
	str++;
	flag->flag_bytes_flag++;
	return (str);
}

const char	*check_flag_space(t_flag *flag, const char *str)
{
	flag->flag_space = 1;
	str++;
	flag->flag_bytes_flag++;
	return (str);
}

const char	*check_flag_zero(t_flag *flag, const char *str)
{
	ssize_t	count;
	char	buf[8];
	int		is_first;

	count = 0;
	is_first = 1;
	str++;
	flag->flag_bytes_flag++;
	while (*str >= '0' && *str <= '9')
	{
		is_first = 0;
		buf[count++] = *str;
		str++;
		flag->flag_bytes_flag++;
	}
	buf[count] = '\0';
	if (is_first)
	{
		flag->flag_zero = 0;
		return (str);
	}
	flag->flag_zero = ft_atoi(buf);
	return (str);
}

const char	*check_flag_period(t_flag *flag, const char *str)
{
	ssize_t	count;
	char	buf[8];
	int		is_first;

	count = 0;
	is_first = 1;
	str++;
	flag->flag_bytes_flag++;
	while (*str >= '0' && *str <= '9')
	{
		is_first = 0;
		buf[count++] = *str;
		str++;
		flag->flag_bytes_flag++;
	}
	buf[count] = '\0';
	if (is_first)
	{
		flag->flag_period = 0;
		return (str);
	}
	flag->flag_period = ft_atoi(buf);
	if (flag->flag_zero != 0)
		flag->flag_right = flag->flag_period;
	return (str);
}
