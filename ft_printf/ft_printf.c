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

#include "ft_printf.h"

int	print_param(const char c, va_list args)
{
	if (c == 'c')
		return (case_c(args));
	else if (c == 's')
		return (case_s(args));
	else if (c == 'p')
		return (case_p(args));
	else if (c == 'd' || c == 'i')
		return (case_d_i(args));
	else if (c == 'u')
		return (case_u(args));
	else if (c == 'x')
		return (case_x(args));
	else if (c == 'X')
		return (case_large_x(args));
	else
		return (ft_putchar(c), 1);
}

int	ft_printf(const char *str, ...)
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

// #include <stdio.h>

// int	main(void)
// {
// 	int			result;
// 	int			result2;
// 	// char		c = '\0';
// 	const char	str[] = "";
// 	void		*p;
// 	int			i;
// 	unsigned int	ui;

// 	i = -21474836;
// 	ui = -1453543;
// 	p = (void *)str;
// 	printf("my:\n");
// 	result = ft_printf("%p\n", p);
// 	printf("%d\n", result);
// 	printf("orig:\n");
// 	result2 = printf("%p\n", p);
// 	printf("%d\n", result2);
// 	return (0);
// }
