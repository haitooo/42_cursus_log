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

int	ft_fprintf(const char *str, ...);
int	fprint_param(const char c, va_list args);
int	ft_fputchar(char c);
int	ft_fputstr(const char *s);
int	ft_fputnbr(int n);
int	ft_fputnbr_long(long n);
int	ft_fputunbr(unsigned int n);
int	ft_fprint_addr(unsigned char *addr_hex);
int	fcase_c(va_list args);
int	fcase_s(va_list args);
int	fcase_p(va_list args);
int	fcase_d_i(va_list args);
int	fcase_ld(va_list args);
int	fcase_u(va_list args);
int	fcase_x(va_list args);
int	fcase_large_x(va_list args);

#endif
