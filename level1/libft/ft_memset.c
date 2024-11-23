/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 21:32:03 by haito             #+#    #+#             */
/*   Updated: 2024/10/26 06:18:12 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*start;

	start = s;
	while (n--)
		*start++ = c;
	return (s);
}

// #include<string.h>
// #define BUF_SIZE 16

// int main(void)
// {
// 	char	buf1[BUF_SIZE];
// 	char	buf2[BUF_SIZE];

// 	memset(buf1, '\0', BUF_SIZE);
// 	ft_memset(buf2, '\0', BUF_SIZE);
// 	printf("< after init >\n");
// 	printf("buf1:%s\n", buf1);
// 	printf("buf2:%s\n", buf2);

// 	memset(buf1, 'a', 4);
// 	memset(buf1, 'b', 2);
// 	ft_memset(buf2, 'a', 4);
// 	ft_memset(buf2, 'b', 2);

// 	printf("< after set >\n");
// 	printf("buf1:%s\n", buf1);
// 	printf("buf2:%s\n", buf2);
// 	return (0);
// }
