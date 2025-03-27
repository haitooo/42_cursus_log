/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_set_stdio.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 23:08:55 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/22 17:10:05 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_save_stdio(t_tokens **tokens, t_saved *saved)
{
	t_tokens	*head;

	saved->in = -1;
	saved->out = -1;
	head = *tokens;
	while (head && (saved->in == -1 || saved->out == -1))
	{
		if ((head->type == INPUT || head->type == HEREDOC) && saved->in == -1)
		{
			saved->in = dup(STDIN_FILENO);
			if (saved->in == -1)
				return (builtin_error(errno, "dup stdin"));
		}
		if ((head->type == TRUNC || head->type == APPEND) && saved->out == -1)
		{
			saved->out = dup(STDOUT_FILENO);
			if (saved->out == -1)
				return (builtin_error(errno, "dup stdout"));
		}
		head = head->next;
	}
	return (SUCCESS);
}

int	builtin_reset_stdio(t_saved *saved)
{
	if (saved->in != -1)
	{
		if (dup2(saved->in, STDIN_FILENO) == -1)
			return (builtin_error(errno, "dup stdin"));
		if (close(saved->in) == -1)
			return (builtin_error(errno, "close saved stdin"));
	}
	if (saved->out != -1)
	{
		if (dup2(saved->out, STDOUT_FILENO) == -1)
			return (builtin_error(errno, "dup stdout"));
		if (close(saved->out) == -1)
			return (builtin_error(errno, "close saved stdout"));
	}
	return (SUCCESS);
}
