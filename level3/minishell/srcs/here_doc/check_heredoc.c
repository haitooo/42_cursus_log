/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 01:09:59 by haito             #+#    #+#             */
/*   Updated: 2025/03/31 05:34:01 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	delete_tmpfile(t_status *st)
{
	while (st)
	{
		if (st->heredoc)
			unlink(st->heredoc);
		st = st->next;
	}
}

void	free_heres(t_status *st)
{
	unlink(st->heredoc);
	free(st->heredoc);
	st->heredoc = NULL;
}

int	check_heredoc2(t_status *st, t_tokens *token, t_var **var,
		t_status **st_head)
{
	while (token)
	{
		if (token->type == HEREDOC)
		{
			if (!token->next)
				return (error_check_heredoc(var));
			if (st->heredoc)
				free_heres(st);
			st->heredoc = here_doc(token->next->token, var, st_head);
			if (!st->heredoc)
				return (ERROR);
			if (token->next->next)
			{
				token = token->next->next;
				continue ;
			}
			else
				break ;
		}
		token = token->next;
	}
	return (0);
}

int	check_heredoc(t_status **st_head, t_var **var)
{
	t_status	*st;
	t_tokens	*token;
	int			heredoc_result;

	st = *st_head;
	while (st)
	{
		errno = 0;
		st->token = expander(st->cmds, var);
		if (!st->token)
		{
			if (errno)
				return (ERROR);
			return (0);
		}
		token = st->token;
		heredoc_result = check_heredoc2(st, token, var, st_head);
		if (heredoc_result == ERROR)
			return (ERROR);
		st = st->next;
	}
	return (0);
}
