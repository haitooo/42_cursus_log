/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 21:47:38 by haito             #+#    #+#             */
/*   Updated: 2024/10/26 05:21:00 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_bzero(void *s, size_t n)
{
	unsigned char	*start;

	start = s;
	ft_memset(start, '\0', n);
	return (start);
}

// #include<string.h>
// #define BUF_SIZE 4

// int main(void)
// {
// 	char	buf1[BUF_SIZE];
// 	char	buf2[BUF_SIZE];
// 	size_t	i;

// 	bzero(buf1, BUF_SIZE);
// 	ft_bzero(buf2, BUF_SIZE);
// 	printf("< orgi >\n");
// 	i = 0;
// 	while (i < BUF_SIZE)
// 	{
// 		printf("buffer[%zu] = %d\n", i, buf1[i]);
// 		i++;
// 	}
// 	printf("< self >\n");
// 	i = 0;
// 	while (i < BUF_SIZE)
// 	{
// 		printf("buffer[%zu] = %d\n", i, buf2[i]);
// 		i++;
// 	}
// 	return (0);
// }
