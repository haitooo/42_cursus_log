/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_empty_tokens.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 14:21:12 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/26 20:36:01 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_tokens	*remove_empty_top_tokens(t_tokens **tokens)
{
	t_tokens	*tmp;

	while (*tokens && !(*tokens)->token[0])
	{
		tmp = *tokens;
		*tokens = (*tokens)->next;
		free(tmp->token);
		free(tmp);
	}
	return (*tokens);
}

static t_tokens	*remove_empty_tokens_in_between(t_tokens **tokens)
{
	t_tokens	*head;
	t_tokens	*prev;
	t_tokens	*tmp;

	prev = NULL;
	head = *tokens;
	if (!head)
		return (NULL);
	while (head)
	{
		if (!head->token || !head->token[0])
		{
			tmp = head;
			prev->next = head->next;
			head = head->next;
			free(tmp->token);
			free(tmp);
		}
		else
		{
			prev = head;
			head = head->next;
		}
	}
	return (*tokens);
}

t_tokens	*remove_empty_tokens(t_tokens **tokens)
{
	t_tokens	*new_head;

	new_head = remove_empty_top_tokens(tokens);
	new_head = remove_empty_tokens_in_between(&new_head);
	return (new_head);
}
