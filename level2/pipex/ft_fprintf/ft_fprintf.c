/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 13:39:20 by haito             #+#    #+#             */
/*   Updated: 2024/11/23 19:28:08 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_fprintf.h"

int	ft_fputstr(const char *s)
{
	int	result;

	result = 0;
	while (*s)
	{
		if (write(2, s++, 1) == -1)
			return (-1);
		result += 1;
	}
	return (result);
}

int	ft_fputchar(char c)
{
	return (write(2, &c, 1));
}

int	ft_fprintf(const char *str, ...)
{
	int		result;
	va_list	args;
	ssize_t	bytes_written;

	result = 0;
	va_start(args, str);
	while (*str)
	{
		if (*str == '%' && *(str + 1) != '\0')
		{
			str++;
			bytes_written = print_param(*str++, args);
			if (bytes_written == -1)
				return (-1);
			result += bytes_written;
			continue ;
		}
		if (ft_putchar(*str) == -1)
			return (-1);
		str++;
		result += 1;
	}
	va_end(args);
	return (result);
}
