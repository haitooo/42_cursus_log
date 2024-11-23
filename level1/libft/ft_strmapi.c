/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 02:16:59 by haito             #+#    #+#             */
/*   Updated: 2024/10/26 02:44:25 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(const char *s, char (*f)(unsigned int, char))
{
	size_t			sizeof_str;
	char			*result;
	unsigned int	i;

	if (!s || !f)
		return (0);
	i = 0;
	sizeof_str = ft_strlen(s);
	result = (char *)malloc(sizeof_str + 1);
	if (!result)
		return (0);
	while (i < sizeof_str)
	{
		result[i] = f(i, s[i]);
		i++;
	}
	result[sizeof_str] = '\0';
	return (result);
}

// char	ft_upper(unsigned int i, char c)
// {
// 	(void)i;
// 	if (c >= 'a' && c <= 'z')
// 		c -= 32;
// 	return (c);
// }

// int	main(void)
// {
// 	char	str[] = "abc4def";
// 	char	*result;

// 	printf("before:%s\n", str);
// 	result = ft_strmapi(str, &ft_upper);
// 	printf("after:%s\n", result);
// 	return (0);
// }
