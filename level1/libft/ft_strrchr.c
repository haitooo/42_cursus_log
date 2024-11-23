/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 23:22:14 by haito             #+#    #+#             */
/*   Updated: 2024/10/26 06:42:33 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int c)
{
	size_t			sizeof_str;
	const char		*str_;
	unsigned char	ch;

	str_ = str;
	ch = (unsigned char)c;
	sizeof_str = ft_strlen(str_);
	str_ += sizeof_str;
	sizeof_str += 1;
	while (sizeof_str--)
	{
		if (*str_ == ch)
			return ((char *)str_);
		str_--;
	}
	return (NULL);
}

// #include  <string.h>

// int	main(void)
// {
// 	char	str[] = "abcdcef";
// 	char	c = 'c';
// 	char	*result;
// 	char	*result_;

// 	result = ft_strrchr(str, c);
// 	result_ = strrchr(str, c);
// 	printf("%s\n", result);
// 	printf("%s\n", result_);
// 	return (0);
// }
