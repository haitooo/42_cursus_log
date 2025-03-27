/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 01:03:02 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/23 23:49:04 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	isvalid_type(t_type prev, char *str)
{
	if (prev != APPEND && prev != TRUNC && prev != INPUT && prev != HEREDOC)
		return (SUCCESS);
	if (ft_strcmp(str, "*"))
		return (SUCCESS);
	return (ERROR);
}

void	expand_wildcard(t_tokens **tokens)
{
	t_tokens	*head;
	t_type		prev;

	head = *tokens;
	prev = WORD;
	while (head)
	{
		if (isvalid_type(prev, head->token) != ERROR && check_wild(head->token))
			head = open_wildcard(&head);
		if (head)
		{
			prev = head->type;
			head = head->next;
		}
	}
}
