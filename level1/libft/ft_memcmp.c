/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 18:25:47 by haito             #+#    #+#             */
/*   Updated: 2024/10/25 22:10:53 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned const char	*s1_;
	unsigned const char	*s2_;

	s1_ = s1;
	s2_ = s2;
	while (n--)
	{
		if (*s1_ != *s2_)
			return (*s1_ - *s2_);
		s1_++;
		s2_++;
	}
	return (0);
}

// #include <string.h>

// int	main(void)
// {
// 	// char	buf1[] = "aaadbbcc";
// 	// char	buf2[] = "aaabbbccc";
// 	int		result;
// 	int		result_ ;
// 	// char s[] = {-128, 0, 127, 0};
// 	char s2[] = {0, 0, 127, 0};
// 	char s3[] = {0, 0, 42, 0};

// 	result_ = memcmp(s2, s3, 4);
// 	result = ft_memcmp(s2, s3, 4);

// 	printf("self buf3:%d\n", result);
// 	printf("orig buf2:%d\n", result_);
// 	return (0);
// }
