/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 19:21:04 by haito             #+#    #+#             */
/*   Updated: 2024/10/22 21:11:29 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (2048);
	return (0);
}

//戻り値　digit　→　2048

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
// 	result = ft_isdigit(c);
// 	result_orig = isdigit(c);
// 	printf("self:%d\norig:%d\n", result, result_orig);
// 	return (0);
// }
