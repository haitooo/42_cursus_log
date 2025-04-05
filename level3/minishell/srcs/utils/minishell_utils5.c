/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils5.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 17:49:36 by haito             #+#    #+#             */
/*   Updated: 2025/04/04 00:40:32 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_strncpy(char *dest, const char *src, size_t size)
{
	size_t	sizeof_src;
	size_t	i;

	i = 0;
	sizeof_src = ft_strlen(src);
	while (i < size && src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	while (i < size)
		dest[i++] = '\0';
	return (sizeof_src);
}

int	ft_isdigit(int c)
{
	return ('0' <= c && c <= '9');
}

int	ft_isalnum(int c)
{
	return (ft_isdigit(c) || ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'));
}

int	ft_atoi_exit(const char *str)
{
	int				is_nega;
	unsigned long	result;
	const char		*str_start;

	str_start = str;
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
		if (is_nega == -1 && result == (unsigned long)LONG_MAX + 1)
			return ((int)LONG_MIN);
		if (result > LONG_MAX)
			return (ft_eprintf
				("minishell: exit: %s: numeric argument required\n", str_start),
				SYNERR);
	}
	return ((int)(result * is_nega));
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
