/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 22:18:03 by haito             #+#    #+#             */
/*   Updated: 2024/10/26 06:17:28 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*dest_;
	const unsigned char	*src_;

	dest_ = dest;
	src_ = src;
	while (n--)
		*dest_++ = *src_++;
	return (dest);
}

// #include <string.h>
// #define BUF_SIZE 16

// int main(void)
// {
// 	char	buf1[BUF_SIZE];
// 	char	buf2[BUF_SIZE];
// 	char	buf3[BUF_SIZE];

// 	memset(buf1, '\0', BUF_SIZE);
// 	memset(buf2, '\0', BUF_SIZE);
// 	memset(buf3, '\0', BUF_SIZE);
// 	printf("< after init >\n");
// 	printf("buf1:%s\n", buf1);
// 	printf("buf2:%s\n", buf2);
// 	printf("buf3:%s\n", buf3);

// 	memset(buf1, 'a', 4);
// 	memset(buf1, 'b', 2);

// 	printf("< after set >\n");
// 	printf("buf1:%s\n", buf1);

// 	memcpy(buf2, buf1, 3);
// 	ft_memcpy(buf3, buf1, 3);

// 	printf("< after cpy >\n");
// 	printf("orig buf2:%s\n", buf2);
// 	printf("self buf3:%s\n", buf3);

// 	return (0);
// }
