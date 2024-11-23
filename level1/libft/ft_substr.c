/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 23:56:44 by haito             #+#    #+#             */
/*   Updated: 2024/10/26 00:42:30 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	char	*dest;
	size_t	sizeof_str;

	sizeof_str = ft_strlen(s);
	if (start >= sizeof_str)
		return (ft_strdup(""));
	if (start + len > sizeof_str)
		len = sizeof_str - start;
	dest = (char *)malloc(len + 1);
	if (!dest)
		return (0);
	ft_strncpy(dest, s + start, len);
	dest[len] = '\0';
	return (dest);
}

// int	main(void)
// {
// 	char	s[] = "abcdefghijklnm";
// 	char	*result;

// 	result = ft_substr(s, 3, 4);
// 	printf("%s\n", result);
// 	return (0);
// }
