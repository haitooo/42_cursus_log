/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_flag.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 12:58:51 by haito             #+#    #+#             */
/*   Updated: 2024/11/25 20:07:37 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

const char	*check_flag_right_align(t_flag *flag, const char *str)
{
	ssize_t	count;
	char	buf[8];

	count = 0;
	while (*str >= '0' && *str <= '9')
	{
		buf[count++] = *str;
		str++;
		flag->flag_bytes_flag++;
	}
	buf[count] = '\0';
	flag->flag_right = ft_atoi(buf);
	return (str);
}

const char	*check_flag_left_align(t_flag *flag, const char *str)
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
		flag->flag_left = 0;
		return (str);
	}
	flag->flag_left = ft_atoi(buf);
	return (str);
}

const char	*check_flag(t_flag *flag, const char *str)
{
	while (*str == '-' || *str == '0' || *str == '.'
		|| *str == '#' || *str == '+' || *str == ' '
		|| (*str >= '0' && *str <= '9'))
	{
		if (*str == '0')
			str = check_flag_zero(flag, str);
		else if (*str == '-')
			str = check_flag_left_align(flag, str);
		else if (*str >= '1' && *str <= '9')
			str = check_flag_right_align(flag, str);
		else if (*str == '.')
			str = check_flag_period(flag, str);
		else if (*str == '#')
			str = check_flag_hash(flag, str);
		else if (*str == '+')
			str = check_flag_plus(flag, str);
		else if (*str == ' ')
			str = check_flag_space(flag, str);
	}
	if ((flag->flag_left != 0 || flag->flag_period != -1)
		&& !(flag->flag_zero && flag->flag_period != -1))
		flag->flag_zero = 0;
	return (str);
}
