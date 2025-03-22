/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_vars_split.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:42:08 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/19 17:43:49 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*heredoc_dup_var(char *str, char **words, int *i, t_var **varlist)
{
	char	*end;
	char	*name;
	t_var	*var;

	end = str;
	end++;
	while (*end && (ft_isalnum(*end) || *end == '_'))
		end++;
	name = ft_strndup(str + 1, end - str - 1);
	if (!name)
		return (NULL);
	var = get_var(varlist, name);
	free(name);
	if (!var)
		words[*i] = ft_strdup("");
	else
		words[*i] = ft_strdup(var->value);
	if (!words[*i])
		return (NULL);
	*i += 1;
	return (end);
}

static char	*dup_plain_text(char *str, char **words, int *i)
{
	char	*end;

	end = str;
	while (*end == '$')
		end++;
	while (*end && *end != '$')
		end++;
	words[*i] = ft_strndup(str, end - str);
	if (!words[*i])
		return (NULL);
	*i += 1;
	return (end);
}

char	**heredoc_split_token(char *token, int malloc_size, t_var **varlist)
{
	char	**words;
	int		i;

	words = (char **)malloc(sizeof(char *) * (malloc_size + 1));
	if (!words)
		return (NULL);
	i = 0;
	while (*token)
	{
		if (*token == '$' && (ft_isalnum(*(token + 1)) || *(token + 1) == '_'))
			token = heredoc_dup_var(token, words, &i, varlist);
		else
			token = dup_plain_text(token, words, &i);
		if (!token)
		{
			free_words(words, malloc_size);
			return (NULL);
		}
	}
	words[i] = NULL;
	return (words);
}
