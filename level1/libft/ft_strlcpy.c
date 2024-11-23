/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 19:27:59 by haito             #+#    #+#             */
/*   Updated: 2024/10/23 20:50:34 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	sizeof_src;
	size_t	i;

	sizeof_src = 0;
	i = 0;
	sizeof_src = ft_strlen(src);
	if (size == 0)
		return (sizeof_src);
	while (src[i] != '\0' && i < (size - 1))
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (sizeof_src);
}

	// 戻り値はsrcの長さ
	// srcからdstにsize-1コピーしてくれる。マイナス1する理由は最後にnull文字を入れる為。

// int	main(void)
// {

// 	char	dest[] = "1234567890";
// 	char	array[] = "abcdefghijk";
// 	ft_strlcpy(dest, array, 5);
// 	printf("%s", dest);
// 	return (0);
// }
