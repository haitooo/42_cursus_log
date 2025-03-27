/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 12:03:58 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/22 10:54:59 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_strs(char **strs)
{
	int	i;

	i = 0;
	if (!strs)
		return ;
	while (strs[i])
	{
		free(strs[i]);
		i++;
	}
	free(strs);
}

void	free_tokens(t_tokens **tokens)
{
	t_tokens	*head;
	t_tokens	*tmp;

	if (!tokens)
		return ;
	head = *tokens;
	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp->token);
		free(tmp);
	}
}

void	free_one_token(t_tokens **tokens, t_tokens *rm_token)
{
	t_tokens	*head;

	if (!tokens)
		return ;
	head = *tokens;
	while (head && head->next != rm_token)
		head = head->next;
	if (!head)
		return ;
	if (head == rm_token)
		*tokens = (*tokens)->next;
	else
		head->next = head->next->next;
	free(rm_token->token);
	free(rm_token);
}

void	free_varlist(t_var **varlist)
{
	t_var	*head;
	t_var	*tmp;

	if (!varlist)
		return ;
	head = *varlist;
	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp->name);
		free(tmp->value);
		free(tmp);
	}
}

void	free_words(char **words, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		free(words[i]);
		i++;
	}
	free(words);
}
