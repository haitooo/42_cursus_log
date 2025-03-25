/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand_vars.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 13:41:50 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/22 06:34:45 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*concat_words(char **words, int size)
{
	char	*expanded_token;
	size_t	total_len;
	int		i;

	total_len = 0;
	i = 0;
	while (i < size)
	{
		total_len += ft_strlen(words[i]);
		i++;
	}
	expanded_token = (char *)malloc(sizeof(char) * (total_len + 1));
	if (!expanded_token)
		return (NULL);
	expanded_token[0] = '\0';
	i = 0;
	while (i < size)
	{
		if (words[i])
			ft_strlcat(expanded_token, words[i], total_len + 1);
		i++;
	}
	free_words(words, size);
	return (expanded_token);
}

static char	**dup_null(void)
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
	return (words);
}

char	*heredoc_expand_var(char *buf, t_var **varlist)
{
	int		words_size;
	char	**words;

	words_size = heredoc_vars_count(buf);
	if (!words_size)
	{
		words = dup_null();
		words_size = 1;
	}
	else
		words = heredoc_split_token(buf, words_size, varlist);
	if (!words)
		return (NULL);
	free(buf);
	buf = concat_words(words, words_size);
	if (!buf)
		return (NULL);
	return (buf);
}
