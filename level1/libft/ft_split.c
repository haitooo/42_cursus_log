/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 00:51:02 by haito             #+#    #+#             */
/*   Updated: 2024/10/26 04:59:00 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	my_wordlen(const char *str, char c)
{
	int	count;

	count = 0;
	while (str[count])
	{
		if (str[count] == c)
			return (count);
		count++;
	}
	return (count);
}

static int	count_words(const char *str, char c)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (str[i])
	{
		while (str[i] && str[i] == c)
			i++;
		if (str[i])
			count++;
		while (str[i] && str[i] != c)
			i++;
	}
	return (count);
}

static char	*ft_word(const char *str, char c)
{
	int		len_word;
	int		i;
	char	*word;

	i = 0;
	len_word = my_wordlen(str, c);
	word = (char *)malloc((len_word + 1) * sizeof(char));
	if (word == 0)
		return (0);
	while (i < len_word)
	{
		word[i] = str[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

static char	**ft_free(char **ret, int n)
{
	while (n--)
		free(ret[n]);
	free(ret);
	return (0);
}

char	**ft_split(const char *str, char c)
{
	char	**ret;
	int		n;

	ret = (char **)malloc((count_words(str, c) + 1) * sizeof(char *));
	if (ret == NULL)
		return (NULL);
	n = 0;
	while (*str)
	{
		while (*str && *str == c)
			str++;
		if (*str)
		{
			ret[n] = ft_word(str, c);
			if (ret[n] == 0)
				return (ft_free(ret, n));
			n++;
		}
		while (*str && *str != c)
			str++;
	}
	ret[n] = 0;
	return (ret);
}
