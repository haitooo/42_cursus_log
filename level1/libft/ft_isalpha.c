/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalpha.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 18:42:52 by haito             #+#    #+#             */
/*   Updated: 2024/10/22 21:11:13 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1024);
	return (0);
}

//戻り値　アルファベット→0以外（本家1024になってた）

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
// 	result = ft_isalpha(c);
// 	result_orig = isalpha(c);
// 	printf("self:%d\norig:%d\n", result, result_orig);
// 	return (0);
// }
