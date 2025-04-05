/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 14:21:12 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/30 21:29:41 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_redirect(t_type type)
{
	if (type == APPEND || type == TRUNC || type == HEREDOC || type == INPUT)
		return (1);
	return (0);
}

static int	is_var(char *str, t_var **varlist)
{
	int		len;
	char	*name;

	if (!ft_strcmp(str, "*"))
		return (1);
	if (str[0] == '$' && (ft_isalpha(str[1]) || str[1] == '_'))
	{
		len = 1;
		while (ft_isalpha(str[len]) || str[len] == '_')
			len++;
		name = ft_strndup(&str[1], len - 1);
		if (!name)
			return (0);
		if (!get_var(varlist, name))
			return (free(name), 1);
		free(name);
	}
	return (0);
}

static t_tokens	*set_next_tokens(t_tokens **start, t_tokens **now,
		t_tokens **new, t_tokens **prev)
{
	if (*start == *now)
		*start = *new;
	else
		(*prev)->next = *new;
	free((*now)->token);
	free(*now);
	return (*new);
}

t_tokens	*expand_tokens(t_tokens **tokens, t_var **varlist)
{
	t_tokens	*head;
	t_tokens	*start;
	t_tokens	*prev;
	t_tokens	*new_tokens;

	head = *tokens;
	start = *tokens;
	prev = NULL;
	while (head)
	{
		if ((prev && is_redirect(prev->type)) && is_var(head->token, varlist))
			head->type = VAR;
		else if (!is_redirect(head->type))
		{
			new_tokens = get_new_tokens(head->token, &(head->next), varlist);
			if (new_tokens)
				head = set_next_tokens(&start, &head, &new_tokens, &prev);
		}
		prev = head;
		head = head->next;
	}
	return (start);
}
