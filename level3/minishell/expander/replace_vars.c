/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_vars.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 13:41:50 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/22 10:56:52 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*concat_words(char **words, int size, char **prev_token)
{
	char	*expanded_token;
	size_t	total_len;
	int		i;

	free(*prev_token);
	total_len = 0;
	i = 0;
	while (i < size)
	{
		if (words[i] && words[i][0])
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
		if (words[i] && words[i][0])
			ft_strlcat(expanded_token, words[i], total_len + 1);
		i++;
	}
	return (expanded_token);
}

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

static int	isvalid_var(t_type type, char *token, char **words, int size)
{
	int	i;
	int	j;
	int	count;

	if (token[0] != '$' || type == WORD || type == HAVE_QUOTE || type == VAR)
		return (SUCCESS);
	i = -1;
	while (token[++i])
		if (token[i] == '\'' || token[i] == '\"')
			return (SUCCESS);
	i = 0;
	count = 0;
	while (i < size)
	{
		j = 0;
		if (words[i])
			while (words[i][j])
				if (ft_isspace(words[i][j++]))
					return (ERROR);
		count += j;
		i++;
	}
	if (!count)
		return (ERROR);
	return (SUCCESS);
}

int	replace_vars(t_tokens **tokens, t_var **varlist)
{
	t_tokens	*head;
	char		**words;
	int			words_size;
	t_type		prev_type;

	head = *tokens;
	prev_type = 0;
	while (head)
	{
		words_size = count_words_and_vars(head->token);
		if (!words_size)
			words = dup_null(&words_size);
		else
			words = split_token(head->token, words_size, varlist);
		if (!words)
			return (ERROR);
		if (!isvalid_var(prev_type, head->token, words, words_size))
			head->token = concat_words(words, words_size, &(head->token));
		else
			head->type = VAR;
		if (!head->token)
			return (ERROR);
		free_words(words, words_size);
		prev_type = head->type;
		head = head->next;
	}
	return (SUCCESS);
}
