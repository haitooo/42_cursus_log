/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 23:31:55 by haito             #+#    #+#             */
/*   Updated: 2024/10/25 18:11:00 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	while ((*s1 || *s2) && n--)
	{
		if (*s1 != *s2)
			return ((unsigned char)*s1 - (unsigned char)*s2);
		s1++;
		s2++;
	}
	return (0);
}

// #include <string.h>

// int	main(void)
// {
// 	char	s1[] = "a";
// 	char	s2[] = "ab";
// 	int		result;
// 	int		result2;
// 	unsigned int n = 4;

// 	result = strncmp(s1, s2, n);
// 	result2 = ft_strncmp(s1, s2, n);
// 	printf("本家：%d\n", result);
// 	printf("自作：%d\n", result2);
// 	return (0);
// }
