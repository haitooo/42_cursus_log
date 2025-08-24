/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 22:39:27 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/01 22:27:22 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	strs_count(char const *str, char c)
{
	int	count;

	count = 1;
	while (*str)
	{
		while (*str == c)
			str++;
		if (*str && *str != c)
		{
			count++;
			while (*str && *str != c)
				str++;
		}
	}
	return (count);
}

static int	ft_strlen_s(char const *str, char c)
{
	int	count;

	count = 0;
	while (str[count] && str[count] != c)
		count++;
	return (count);
}

static void	*all_free(char **ans)
{
	char	**tmp;
	char	*to_free;

	tmp = ans;
	while (*tmp)
	{
		to_free = *tmp;
		tmp++;
		free(to_free);
	}
	free(ans);
	return (NULL);
}

static char	*ft_strdup_c(char const *s, int len)
{
	char	*ans;
	char	*tmp;

	ans = (char *)malloc(sizeof(char) * len + 1);
	if (!ans)
		return (NULL);
	tmp = ans;
	while (len)
	{
		*tmp = *s;
		tmp++;
		s++;
		len--;
	}
	*tmp = '\0';
	return (ans);
}

char	**ft_split(char const *s, char c)
{
	char	**ans;
	char	**tmp;
	int		len;

	if (!s)
		return (NULL);
	ans = (char **)malloc(sizeof(char *) * strs_count(s, c));
	if (!ans)
		return (NULL);
	tmp = ans;
	while (*s)
	{
		while (*s == c)
			s++;
		len = ft_strlen_s(s, c);
		if (!len)
			break ;
		*tmp = ft_strdup_c(s, len);
		if (!*tmp)
			return (all_free(ans));
		s += len;
		tmp++;
	}
	*tmp = NULL;
	return (ans);
}
