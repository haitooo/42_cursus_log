/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_eprintf.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 13:37:03 by haito             #+#    #+#             */
/*   Updated: 2025/03/15 02:10:05 by haito            ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#ifndef FT_EPRINTF_H
# define FT_EPRINTF_H

# include <unistd.h>
# include <stdlib.h>
# include <stdarg.h>
# include <limits.h>

int	ft_eprintf(const char *str, ...);
int	eprint_param(const char c, va_list args);
int	ft_eputchar(char c);
int	ft_eputstr(const char *s);
int	ft_eputnbr(int n);
int	ft_eputnbr_long(long n);
int	ft_eputunbr(unsigned int n);
int	ft_eprint_addr(unsigned char *addr_hex);
int	ecase_c(va_list args);
int	ecase_s(va_list args);
int	ecase_p(va_list args);
int	ecase_d_i(va_list args);
int	ecase_ld(va_list args);
int	ecase_u(va_list args);
int	ecase_x(va_list args);
int	ecase_large_x(va_list args);

#endif
