/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 00:30:18 by haito             #+#    #+#             */
/*   Updated: 2024/10/26 00:44:10 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(const char *s1, const char *s2)
{
	size_t	total_len;
	char	*result;
	char	*p;

	total_len = ft_strlen(s1) + ft_strlen(s2);
	result = (char *)malloc(total_len + 1);
	if (!result)
		return (0);
	p = result;
	while (*s1)
		*p++ = *s1++;
	while (*s2)
		*p++ = *s2++;
	*p = '\0';
	return (result);
}

// int	main(void)
// {
// 	char	s1[] = "abc";
// 	char	s2[] = "123";
// 	char	*result;

// 	result = ft_strjoin(s1, s2);
// 	printf("%s\n", result);
// 	return (0);
// }
