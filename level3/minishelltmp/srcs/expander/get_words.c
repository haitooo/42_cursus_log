/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_words.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 00:47:57 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/24 01:13:26 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**dup_null(int *words_size)
{
	char	**words;

	words = (char **)malloc(sizeof(char *) * 2);
	if (!words)
		return (NULL);
	words[0] = ft_strdup("");
	if (!words[0])
	{
		free(words);
		return (NULL);
	}
	words[1] = NULL;
	*words_size = 1;
	return (words);
}

static char	**make_empty_words(int malloc_size)
{
	char	**words;
	int		i;

	words = (char **)malloc(sizeof(char *) * (malloc_size + 1));
	if (!words)
		return (NULL);
	i = 0;
	while (i < malloc_size)
	{
		words[i] = NULL;
		i++;
	}
	return (words);
}

int	is_var(char *str)
{
	if (*str != '$')
		return (0);
	str++;
	if (*str && (ft_isalnum(*str) || *str == '_' || *str == '?'))
		return (1);
	return (0);
}

static char	**split_token(char *token, int malloc_size, t_var **varlist)
{
	char	**words;
	int		i;

	words = make_empty_words(malloc_size);
	if (!words)
		return (NULL);
	i = 0;
	while (*token)
	{
		if (is_var(token))
		{
			token = dup_var(token, words, WORD, varlist);
			i++;
		}
		else if (*token == '\'')
			token = dup_singlequot_text(token, words, &i);
		else if (*token == '\"')
			token = dup_doublequot_text(token, words, &i, varlist);
		else
			token = dup_plain_text(token, words, &i);
		if (!token)
			return (free_words(words, malloc_size), NULL);
	}
	return (words);
}

char	**get_words(char *str, t_var **varlist, int words_size)
{
	char	**words;

	if (!words_size)
		words = dup_null(&words_size);
	else
		words = split_token(str, words_size, varlist);
	return (words);
}
