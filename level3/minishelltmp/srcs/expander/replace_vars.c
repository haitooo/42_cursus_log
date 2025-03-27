/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_vars.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 13:41:50 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/25 13:37:32 by tssaito          ###   ########.fr       */
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

static int	isvalid_file_for_redirect(int size, char **words)
{
	int	i;
	int	j;

	i = 0;
	while (i < size)
	{
		j = 0;
		if (words[i])
		{
			while (words[i][j])
			{
				if (ft_isspace(words[i][j]))
					return (ERROR);
				j++;
			}
		}
		i++;
	}
	return (SUCCESS);
}

static int	isvalid_var(t_type type, char *token, char **words, int size)
{
	int	i;

	if (token[0] != '$' || type == WORD || type == HAVE_QUOTE || type == VAR)
		return (1);
	else if (type == HEREDOC || !size)
		return (0);
	i = 0;
	while (token[i])
	{
		if (token[i] == '\'' || token[i] == '\"')
			return (1);
		i++;
	}
	if (isvalid_file_for_redirect(size, words) == ERROR)
		return (0);
	return (1);
}

int	replace_vars(t_tokens **tokens, t_var **varlist)
{
	t_tokens	*head;
	char		**words;
	t_type		prev_type;
	int			words_size;

	head = *tokens;
	prev_type = 0;
	while (head)
	{
		words_size = count_words_and_vars(head->token);
		words = get_words(head->token, varlist, words_size);
		if (!words)
			return (ERROR);
		if (isvalid_var(prev_type, head->token, words, words_size))
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
