/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_eprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 13:39:20 by haito             #+#    #+#             */
/*   Updated: 2025/03/22 18:42:33 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_eprintf.h"

int	eprint_param(const char c, va_list args)
{
	if (c == 'c')
		return (ecase_c(args));
	else if (c == 's')
		return (ecase_s(args));
	else if (c == 'p')
		return (ecase_p(args));
	else if (c == 'd' || c == 'i')
		return (ecase_d_i(args));
	else if (c == 'l')
		return (ecase_ld(args));
	else if (c == 'u')
		return (ecase_u(args));
	else if (c == 'x')
		return (ecase_x(args));
	else if (c == 'X')
		return (ecase_large_x(args));
	else
		return (ft_eputchar(c), 1);
}

int	ft_eprintf(const char *str, ...)
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
			bytes_written = eprint_param(*str++, args);
			if (bytes_written == -1)
				return (-1);
			result += bytes_written;
			continue ;
		}
		if (ft_eputchar(*str) == -1)
			return (-1);
		str++;
		result += 1;
	}
	va_end(args);
	return (result);
}
