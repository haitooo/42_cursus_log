/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:11:35 by haito             #+#    #+#             */
/*   Updated: 2024/10/26 06:33:29 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned const char	*str;
	unsigned char		ch;

	str = (const unsigned char *)s;
	ch = (unsigned char)c;
	while (n--)
	{
		if (*str == ch)
			return ((void *)str);
		str++;
	}
	return (NULL);
}

// #include <string.h>
// #define BUF_SIZE 16

// int	main(void)
// {
// 	char	buf[BUF_SIZE];
// 	char	*result;
// 	char	*result_ ;

// 	memset(buf, '\0', BUF_SIZE);
// 	memset(buf, 'c', 5);
// 	memset(buf, 'a', 4);
// 	memset(buf, 'b', 2);

// 	printf("< after set >\n");
// 	printf("buf1:%s\n", buf);

// 	result_ = memchr(buf, 512 + 'b', 3);
// 	result = ft_memchr(buf, 512 + 'b', 3);

// 	printf("< after cpy >\n");
// 	printf("self buf3:%s\n", result);
// 	printf("orig buf2:%s\n", result_);

// 	return (0);
// }
