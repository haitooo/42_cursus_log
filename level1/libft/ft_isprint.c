/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 21:18:32 by haito             #+#    #+#             */
/*   Updated: 2024/10/22 21:23:13 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isprint(int c)
{
	if (c >= 32 && c <= 126)
		return (16384);
	return (0);
}

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
// 	result = ft_isprint(c);
// 	result_orig = isprint(c);
// 	// result = ft_isprint('\n');
// 	// result_orig = isprint('\n');
// 	printf("self:%d\norig:%d\n", result, result_orig);
// 	return (0);
// }
