/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 21:12:01 by haito             #+#    #+#             */
/*   Updated: 2024/10/26 05:22:10 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalnum(int c)
{
	if (ft_isalpha(c) || ft_isdigit(c))
		return (8);
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
// 	result = ft_isalnum(c);
// 	result_orig = isalnum(c);
// 	printf("self:%d\norig:%d\n", result, result_orig);
// 	return (0);
// }
