/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 18:58:11 by haito             #+#    #+#             */
/*   Updated: 2024/10/22 21:11:17 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isascii(int c)
{
	if (c >= 0 && c <= 127)
		return (1);
	return (0);
}

//戻り値　ascii → 1

// #include <ctype.h>

// int	main(int argc, char **argv)
// {
// 	char	c;
// 	int		result;
// 	int		result_orig;

// 	if (argc != 2)
// 		return (0);
// 	c = (char)argv[1][0];
// 	printf("%d\n%c\n", c, c);
// 	result = ft_isascii(c);
// 	result_orig = isascii(c);
// 	// result = ft_isascii(0x80);
// 	// result_orig = isascii(0x80);
// 	printf("self:%d\norig:%d\n", result, result_orig);
// 	return (0);
// }
