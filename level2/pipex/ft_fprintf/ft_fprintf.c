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

int	fprint_param(const char c, va_list args)
{
	if (c == 'c')
		return (fcase_c(args));
	else if (c == 's')
		return (fcase_s(args));
	else if (c == 'p')
		return (fcase_p(args));
	else if (c == 'd' || c == 'i')
		return (fcase_d_i(args));
	else if (c == 'l')
		return (fcase_ld(args));
	else if (c == 'u')
		return (fcase_u(args));
	else if (c == 'x')
		return (fcase_x(args));
	else if (c == 'X')
		return (fcase_large_x(args));
	else
		return (ft_fputchar(c), 1);
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
			bytes_written = fprint_param(*str++, args);
			if (bytes_written == -1)
				return (-1);
			result += bytes_written;
			continue ;
		}
		if (ft_fputchar(*str) == -1)
			return (-1);
		str++;
		result += 1;
	}
	va_end(args);
	return (result);
}
