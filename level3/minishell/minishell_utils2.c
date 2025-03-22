/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 21:15:06 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/14 13:04:09 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dsize)
{
	size_t	dst_len;
	size_t	src_len;

	src_len = ft_strlen(src);
	if (!dsize)
		return (src_len);
	dst_len = ft_strlen(dst);
	if (dsize <= dst_len)
		return (dsize + src_len);
	ft_strlcpy(dst + dst_len, src, dsize - dst_len);
	return (dst_len + src_len);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t dsize)
{
	size_t	i;
	size_t	len;

	len = ft_strlen(src);
	if (!dsize)
		return (len);
	i = 0;
	while (src[i] && i < dsize - 1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (len);
}

int	ft_isspace(char c)
{
	if (c == ' ' || (9 <= c && c <= 13))
		return (1);
	return (0);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		s1_len;
	int		s2_len;
	char	*ans;

	if (!s1 || !s2)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	ans = (char *)malloc(sizeof(char) * (s1_len + s2_len + 1));
	if (!ans)
		return (NULL);
	ft_strlcpy(ans, s1, s1_len + 1);
	ft_strlcat(ans, s2, s1_len + s2_len + 1);
	return (ans);
}

char	*ft_strjoin_three(char const *s1, char const *s2, char const *s3)
{
	int		s1_len;
	int		s2_len;
	int		s3_len;
	char	*ans;

	if (!s1 || !s2 || !s3)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	s3_len = ft_strlen(s3);
	ans = (char *)malloc(sizeof(char) * (s1_len + s2_len + s3_len + 1));
	if (!ans)
		return (NULL);
	ft_strlcpy(ans, s1, s1_len + 1);
	ft_strlcat(ans, s2, s1_len + s2_len + 1);
	ft_strlcat(ans, s3, s1_len + s2_len + s3_len + 1);
	return (ans);
}
