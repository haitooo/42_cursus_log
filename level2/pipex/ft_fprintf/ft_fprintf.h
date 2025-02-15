/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 13:37:03 by haito             #+#    #+#             */
/*   Updated: 2024/11/23 18:59:09 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_FPRINTF_H
# define FT_FPRINTF_H

# include <unistd.h>
# include <stdlib.h>
# include <stdarg.h>
# include <limits.h>

int	ft_printf(const char *str, ...);
int	print_param(const char c, va_list args);
int	ft_putchar(char c);
int	ft_putstr(const char *s);
int	ft_putnbr(int n);
int	ft_putnbr_long(long n);
int	ft_putunbr(unsigned int n);
int	ft_print_addr(unsigned char *addr_hex);
int	case_c(va_list args);
int	case_s(va_list args);
int	case_p(va_list args);
int	case_d_i(va_list args);
int	case_ld(va_list args);
int	case_u(va_list args);
int	case_x(va_list args);
int	case_large_x(va_list args);

int	ft_fprintf(const char *str, ...);
int	ft_fputchar(char c);
int	ft_fputstr(const char *s);

#endif
