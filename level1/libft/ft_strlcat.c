/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 19:31:46 by haito             #+#    #+#             */
/*   Updated: 2024/10/25 23:26:44 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	sizeof_src;
	size_t	sizeof_dest;

	i = 0;
	sizeof_src = ft_strlen(src);
	sizeof_dest = ft_strlen(dest);
	if (size <= sizeof_dest)
		return (sizeof_src + size);
	while (src[i] != '\0' && (sizeof_dest + i) < (size - 1))
	{
		dest[sizeof_dest + i] = src[i];
		i++;
	}
	dest[sizeof_dest + i] = '\0';
	return (sizeof_dest + sizeof_src);
}

	// 戻り値は連結しようとした文字列の長さ
	// sizeがdestの長さ以下の場合は、size + srcの長さを返す
	// srcを全部連結できたら、dest + secの長さ
	// i < (size - 1）この箇所はバッファサイズを超えないためにNULL文字文を確保するための条件

// int	main(void)
// {
// 	char	s1[] = "ABCDE";
// 	char	s2[] = "123";
// 	char dest[30]; ft_memset(dest, 0, 30);
// 	char * src = (char *)"AAAAAAAAA";
// 	size_t	result;
// 	size_t	result_;

// 	int	i = 0;
// 	while (i < 10)
// 	{
// 		int j = -1;
// 		while (++j < 5)
// 		{
// 			s1[j] = 'A' + j;
// 		}
// 		s1[j] = '\0';
// 		result = ft_strlcat(s1, s2, i);
// 		printf("%s\n", s1);
// 		printf("%zu\n", result);
// 		i++;
// 	}
// 	ft_memset(dest, 'B', 4);
// 	result_ = ft_strlcat(dest, src, 6);
// 	printf("%s\n", dest);
// 	printf("%zu\n", result_);
// 	return (0);
// }
