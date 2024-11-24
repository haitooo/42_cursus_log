/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 13:37:03 by haito             #+#    #+#             */
/*   Updated: 2024/11/25 03:56:11 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <unistd.h>
# include <stdlib.h>
# include <stdarg.h>
# include <limits.h>


#include <stdio.h>


typedef struct s_flag
{
	int		flag_left;
	int		flag_right;
	int		flag_zero;
	int		flag_period;
	int		flag_hash;
	int		flag_plus;
	int		flag_space;
	ssize_t	flag_bytes_flag;
	ssize_t	bytes_written;
}	t_flag;

int			ft_printf(const char *str, ...);
int			case_c(va_list args, t_flag *flag);
int			case_s(va_list args, t_flag *flag);
int			case_p(va_list args, t_flag *flag);
int			case_d_i(va_list args, t_flag *flag);
int			case_d_i_zero(va_list args, t_flag *flag);
int			case_u(va_list args, t_flag *flag);
int			case_x(va_list args, t_flag *flag);
int			case_large_x(va_list args, t_flag *flag);

int			ft_putchar(char c);
int			ft_putstr(const char *s);
int			ft_putnbr(int n, t_flag *flag);
int			ft_putunbr(unsigned int n);
int			ft_atoi(const char *str);
int			ft_isdigit(int c);
size_t		ft_strlen(const char *s);
int			get_digit(int num);
int			get_digit_zero(int num);
int			get_digit_u(unsigned int num);
int			ft_print_addr(unsigned char *addr_hex, t_flag *flag);

const char	*check_flag(t_flag *flag, const char *str);
const char	*check_flag_zero(t_flag *flag, const char *str);

#endif
