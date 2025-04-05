/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_new_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 13:43:06 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/30 17:35:40 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_empty_var(t_splited **splited, t_splited **start)
{
	t_splited	*head;

	head = *splited;
	if (head->str[0])
		return (0);
	if (head->type != PLAINVAR && head->type != QUOTEVAR)
		return (0);
	if (*start != head || head->next)
		return (0);
	free(head->str);
	head->str = NULL;
	return (1);
}

static void	set_next(t_splited **prev, t_splited **next, t_splited **now)
{
	t_splited	*tmp;

	*prev = *next;
	tmp = *now;
	*now = (*now)->next;
	free(tmp->str);
	free(tmp);
}

static t_splited	*remove_empty_vars(t_splited **splited)
{
	t_splited	*start;
	t_splited	*head;
	t_splited	*prev;

	head = *splited;
	start = head;
	while (head)
	{
		if (is_empty_var(&head, &start))
			break ;
		if (!head->str[0] && (head->type == PLAINVAR || head->type == QUOTEVAR))
		{
			if (start == head && head->next)
				set_next(&start, &(head->next), &head);
			else
				set_next(&(prev->next), &(head->next), &head);
		}
		else
		{
			prev = head;
			head = head->next;
		}
	}
	return (start);
}

t_tokens	*get_new_tokens(char *str, t_tokens **next, t_var **varlist)
{
	t_tokens	*new_tokens;
	t_tokens	*head;
	t_splited	*splited;

	splited = split_tokens(pre_split_tokens(str));
	if (!splited)
		return (NULL);
	replace_vars(&splited, varlist);
	splited = remove_empty_vars(&splited);
	new_tokens = get_expanded_tokens(&splited);
	if (!new_tokens)
		return (NULL);
	head = new_tokens;
	while (head && head->next)
		head = head->next;
	head->next = *next;
	return (new_tokens);
}
