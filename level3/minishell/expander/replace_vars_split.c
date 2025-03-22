/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_vars_split.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:42:08 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/20 17:23:30 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*dup_singlequot_text(char *str, char **words, int *i)
{
	char	*end;

	end = str + 1;
	while (*end && *end != '\'')
		end++;
	end++;
	words[*i] = ft_strndup(str, end - str);
	if (!words[*i])
		return (NULL);
	*i += 1;
	if (*end == '\'')
		end++;
	words[*i] = NULL;
	return (end);
}

static char	*dup_doublequot_text(char *str, char **words, int *i,
		t_var **varlist)
{
	char	*end;
	char	*start;

	end = str + 1;
	start = str + 1;
	words[*i] = ft_strdup("\"");
	if (!words[*i])
		return (NULL);
	*i += 1;
	words[*i] = NULL;
	while (*end && *end != '\"')
	{
		if (*start == '$')
			end = dup_var(start, words, HAVE_QUOTE, varlist);
		else
		{
			while (*end && *end != '\"' && *end != '$')
				end++;
			words[*i] = ft_strndup(start, end - start);
			if (!words[*i])
				return (NULL);
		}
		*i += 1;
		start = end;
		words[*i] = NULL;
	}
	words[*i] = ft_strdup("\"");
	if (!words[*i])
		return (NULL);
	*i += 1;
	words[*i] = NULL;
	return (end + 1);
}

static char	*dup_plain_text(char *str, char **words, int *i)
{
	char	*end;

	end = str;
	while (*end == '$')
		end++;
	while (*end && *end != '$' && *end != '\'' && *end != '\"')
		end++;
	words[*i] = ft_strndup(str, end - str);
	if (!words[*i])
		return (NULL);
	*i += 1;
	words[*i] = NULL;
	return (end);
}

static int	is_var(char *str)
{
	if (*str != '$')
		return (0);
	str++;
	if (ft_isalnum(*str) || *str == '_' || *str == '?')
		return (1);
	return (0);
}

char	**split_token(char *token, int malloc_size, t_var **varlist)
{
	char	**words;
	int		i;

	words = (char **)malloc(sizeof(char *) * (malloc_size + 1));
	if (!words)
		return (NULL);
	words[0] = NULL;
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
