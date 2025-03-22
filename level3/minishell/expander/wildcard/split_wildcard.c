/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_wildcard.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 20:33:11 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/22 00:49:25 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_wild_words(char *str)
{
	int	i;
	int	size;

	size = 0;
	while (*str && *str != '/')
	{
		i = 0;
		if (str[i] != '*')
			while (str[i] && str[i] != '*' && str[i] != '/')
				i++;
		else if (str[i] == '*')
			while (str[i] == '*')
				i++;
		size++;
		str += i;
	}
	if (*(str - 1) != '*')
		size++;
	if (*str == '/')
		size++;
	return (size);
}

static char	*set_words(char **words, int *size, char *str, int len)
{
	if (len)
	{
		*words = ft_strndup(str, len);
		if (!*words)
			return (NULL);
	}
	else
	{
		*words = ft_strdup(str);
		if (!*words)
			return (NULL);
	}
	*size += 1;
	return (*words);
}

static int	get_len(char *str)
{
	char	*saved;

	saved = str;
	while (*str && *str != '*' && *str != '/')
		str++;
	return (str - saved);
}

char	**split_wildcards(char *str, int malloc_size)
{
	char	**words;
	int		size;

	words = (char **)malloc(sizeof(char *) * (malloc_size + 1));
	if (!words)
		return (NULL);
	size = 0;
	while (*str && *str != '/')
	{
		if (*str != '*' && !set_words(&words[size], &size, str, get_len(str)))
			return (words);
		while (*str && *str != '*' && *str != '/')
			str++;
		if (*str == '*' && !set_words(&words[size], &size, "*", 0))
			return (words);
		while (*str == '*')
			str++;
	}
	if (words[size - 1][0] != '*' && !set_words(&words[size], &size, "", 0))
		return (words);
	if (*str == '/' && !set_words(&words[size], &size, "/", 0))
		return (words);
	words[size] = NULL;
	return (words);
}
