/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 22:35:56 by haito             #+#    #+#             */
/*   Updated: 2024/10/25 21:43:35 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*dest_;
	const unsigned char	*src_;

	dest_ = dest;
	src_ = src;
	if (dest_ == src_ || n == 0)
		return (dest);
	if (dest_ < src_)
	{
		while (n--)
			*dest_++ = *src_++;
	}
	else
	{
		dest_ += n;
		src_ += n;
		while (n--)
			*(--dest_) = *(--src_);
	}
	return (dest);
}

// #include <string.h>
// #define BUF_SIZE 16

// int main(void)
// {
// 	char	buf1[BUF_SIZE];
// 	char	buf2[BUF_SIZE];
// 	char	buf3[BUF_SIZE];
// 	char s0[] = { 0,  0,  0,  0,  0,  0, 0};
// 	char s[] = {65, 66, 67, 68, 69, 0, 45};

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

// 	int n = 0;
// 	while (n < 8)
// 	{
// 		printf("%d\n", s0[n]);
// 		printf("%p\n", &s0[n]);
// 		n++;
// 	}

// 	// memmove(s0, s, 7);
// 	ft_memmove(s0, s, 7);

// 	n = 0;
// 	while (n < 8)
// 	{
// 		printf("%d\n", s0[n]);
// 		printf("%p\n", &s0[n]);
// 		n++;
// 	}

// 	printf("< after cpy >\n");
// 	printf("orig buf2:%s\n", buf2);
// 	printf("self buf3:%s\n", buf3);

// 	return (0);
// }
