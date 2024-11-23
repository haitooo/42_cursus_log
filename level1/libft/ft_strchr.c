/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 23:12:17 by haito             #+#    #+#             */
/*   Updated: 2024/10/26 06:39:34 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *str, int c)
{
	const unsigned char	*s;
	unsigned char		ch;

	s = (const unsigned char *)str;
	ch = (unsigned char)c;
	while (*s)
	{
		if (*s == ch)
			return ((char *)s);
		s++;
	}
	if (!*s && ch == 0)
		return ((char *)s);
	return (NULL);
}

// int	main(void)
// {
// 	char	str[] = "abcdef";
// 	char	c = 'c';
// 	char	*result;

// 	result = ft_strchr(str, c);
// 	printf("%s\n", result);
// 	return (0);
// }
