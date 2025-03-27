/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   more_tokenizer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 16:30:29 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/26 20:16:51 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_quotes(char **words)
{
	int		i;
	char	ope;
	char	*str;

	str = *words;
	i = 0;
	while (1)
	{
		i = 0;
		while (str[i] && str[i] != '\'' && str[i] != '\"')
			i++;
		if (!str[i])
			break ;
		ope = str[i];
		ft_strlcpy(&str[i], &str[i + 1], ft_strlen(&str[i + 1]) + 1);
		while (str[i] && str[i] != ope)
			i++;
		ft_strlcpy(&str[i], &str[i + 1], ft_strlen(&str[i + 1]) + 1);
		str += i;
	}
}

static t_tokens	*add_new_tokens(t_tokens **prev_token, char **words)
{
	t_tokens	*new;
	t_tokens	*prev;
	int			i;

	prev = *prev_token;
	free(prev->token);
	prev->token = words[0];
	if (!words[1])
		return (prev);
	i = 1;
	while (words[i])
	{
		new = (t_tokens *)malloc(sizeof(t_tokens));
		if (!new)
			return (NULL);
		new->type = WORD;
		new->token = words[i];
		new->next = prev->next;
		prev->next = new;
		prev = new;
		i++;
	}
	return (new);
}

int	remove_quotes_and_split_tokens(t_tokens **tokens, t_type type)
{
	t_tokens	*head;
	char		**words;
	int			i;

	head = *tokens;
	while (head)
	{
		words = split_words(head->token, type);
		if (!words)
			return (ERROR);
		i = 0;
		while (words[i])
		{
			remove_quotes(&words[i]);
			i++;
		}
		head = add_new_tokens(&head, words);
		free(words);
		if (!head)
			return (ERROR);
		head = head->next;
	}
	return (SUCCESS);
}
