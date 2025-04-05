/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 22:11:23 by haito             #+#    #+#             */
/*   Updated: 2025/04/04 00:18:10 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

size_t	ft_strlen(const char *s)
{
	int	count;

	count = 0;
	while (s && s[count])
		count++;
	return (count);
}

int	ft_strlen_sep(const char *str, const char c)
{
	int	count;

	count = 0;
	while (str[count] != c && str[count])
		count++;
	return (count);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	count;

	count = 0;
	while (s1[count] && s2[count])
	{
		if (s1[count] != s2[count])
			return ((unsigned char)s1[count] - (unsigned char)s2[count]);
		count++;
	}
	return ((unsigned char)s1[count] - (unsigned char)s2[count]);
}

int	ft_strncmp(const char *s1, const char *s2, int n)
{
	int	count;

	count = 0;
	while (count < n && s1[count] && s2[count])
	{
		if (s1[count] != s2[count])
			return ((unsigned char)s1[count] - (unsigned char)s2[count]);
		count++;
	}
	if (count == n)
		return (0);
	return ((unsigned char)s1[count] - (unsigned char)s2[count]);
}

char	*ft_strdup(const char *s)
{
	char	*dest;
	int		i;
	int		srclen;

	i = 0;
	srclen = ft_strlen(s);
	dest = (char *)malloc(sizeof(char) * (srclen + 1));
	if (dest == NULL)
		return (NULL);
	while (s[i] != '\0')
	{
		dest[i] = s[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
