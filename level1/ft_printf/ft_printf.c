/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 13:39:20 by haito             #+#    #+#             */
/*   Updated: 2024/11/25 21:54:30 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	init_flag(t_flag *flag)
{
	flag->flag_left = 0;
	flag->flag_right = 0;
	flag->flag_zero = 0;
	flag->flag_period = -1;
	flag->flag_hash = 0;
	flag->flag_plus = 0;
	flag->flag_space = 0;
	flag->flag_bytes_flag = 0;
}

int	print_param(const char *str, va_list args, t_flag *flag)
{
	init_flag(flag);
	str++;
	str = check_flag(flag, str);
	if (*str == 'c')
		return (case_c(args, flag));
	else if (*str == 's')
		return (case_s(args, flag));
	else if (*str == 'p')
		return (case_p(args, flag));
	else if ((*str == 'd' || *str == 'i') && flag->flag_period != -1
		&& !flag->flag_left)
		return (case_d_i_period(args, flag, 0));
	else if ((*str == 'd' || *str == 'i') && flag->flag_zero != 0)
		return (case_d_i_zero(args, flag, 0));
	else if (*str == 'd' || *str == 'i')
		return (case_d_i(args, flag, 0));
	else if (*str == 'u')
		return (case_u(args, flag));
	else if (*str == 'x')
		return (case_x(args, flag));
	else if (*str == 'X')
		return (case_large_x(args, flag));
	else
		return (ft_putchar(*str), 1);
}

int	ft_printf(const char *str, ...)
{
	int		result;
	va_list	args;
	t_flag	flag;
	ssize_t	bytes_written;

	result = 0;
	va_start(args, str);
	while (*str)
	{
		if (*str == '%' && *(str + 1) != '\0')
		{
			bytes_written = print_param(str, args, &flag);
			if (bytes_written == -1)
				return (-1);
			str += (flag.flag_bytes_flag + 2);
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
// 	int				result;
// 	int				result2;
// 	char			c;
// 	const char		str[] = "";
// 	void			*p;
// 	int				i;
// 	unsigned int	ui;

// 	i = 4255;
// 	ui = -1453543;
// 	p = (void *)str;
// 	c = 'a';

// 	printf("my:\n");
// 	result = ft_printf("%-1.9d", 1);
// 	printf("%d\n", result);
// 	printf("orig:\n");
// 	result2 = printf("%-1.9d", 1);
// 	printf("%d\n", result2);
// 	return (0);
// }
