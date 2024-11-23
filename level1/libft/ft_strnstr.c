/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:36:10 by haito             #+#    #+#             */
/*   Updated: 2024/10/25 23:44:01 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	has_little(const char *s1, const char *s2)
{
	while (*s1 && *s2)
	{
		if (*s1 != *s2)
			return ((unsigned char)*s1 - (unsigned char)*s2);
		s1++;
		s2++;
	}
	if (*s2 == '\0')
		return (0);
	else
		return (-1);
}

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	sizeof_little;

	sizeof_little = ft_strlen(little);
	if (little[0] == '\0')
		return ((char *)big);
	while (*big && len >= sizeof_little && len--)
	{
		if (*big == *little && has_little(big, little) == 0)
			return ((char *)big);
		big++;
	}
	return (NULL);
}

// int	main(void)
// {
// 	char	s1[] = "abcdefg";
// 	char	s2[] = "ef";
// 	char	*result;

// 	result = ft_strnstr(s1, s2, 100);
// 	printf("%s\n", result);
// 	return (0);
// }
