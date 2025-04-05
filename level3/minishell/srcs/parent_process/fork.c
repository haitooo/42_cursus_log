/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 01:21:03 by haito             #+#    #+#             */
/*   Updated: 2025/03/31 15:23:29 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_direct_builtin(t_status *st)
{
	if (!st || !st->is_builtin)
		return (0);
	if (st->has_brackets)
		return (0);
	if (st->input_pipefd != -1 || st->output_pipefd != -1)
		return (0);
	if (!st->next)
		return (1);
	if (ft_strcmp(st->token->token, "exit") == 0
		|| ft_strcmp(st->token->token, "export") == 0
		|| ft_strcmp(st->token->token, "unset") == 0
		|| ft_strcmp(st->token->token, "cd") == 0)
		return (1);
	if (st->next->has_and || st->next->has_or)
		return (0);
	if (st->next->has_semicolon)
		return (0);
	return (0);
}

int	check_signal(t_lp *lp)
{
	if (g_signal == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		g_signal = 0;
		lp->result = 130;
		lp->count_forked = 0;
		return (1);
	}
	if (lp->result == 131)
		return (1);
	return (0);
}

int	prepare_and_expand_tokens(t_status *st, t_var **varlist)
{
	if (st->token)
		free_tokens(&(st->token));
	st->token = expander(st->cmds, varlist);
	if (!st->token)
	{
		update_exit_code(0, varlist);
		return (0);
	}
	return (1);
}

int	execute_builtin_or_fork(t_status *st, t_var **varlist, t_lp *lp,
		t_status *st_head)
{
	char	*heredoc;

	heredoc = NULL;
	if (is_direct_builtin(st))
	{
		if (st->heredoc)
		{
			heredoc = ft_strdup(st->heredoc);
			if (!heredoc)
				return (error_node(ERRNO_ONE), ERROR);
		}
		lp->result = call_builtin(&st->token, varlist, st_head, heredoc);
	}
	else
	{
		fork_process(st, varlist, lp, st_head);
		if (!st->next || (!st->next->has_and && !st->next->has_or))
			lp->last_pid = st->pid;
	}
	return (SUCCESS);
}

int	fork_and_wait(t_status **st_head, t_var **varlist)
{
	t_status	*st;
	t_lp		lp;

	st = *st_head;
	lp.count_forked = 0;
	lp.last_pid = -1;
	lp.result = 0;
	while (st)
	{
		if (check_signal(&lp))
			break ;
		if ((st->has_and && lp.result != 0) || (st->has_or && lp.result == 0)
			|| !prepare_and_expand_tokens(st, varlist))
		{
			st = st->next;
			continue ;
		}
		if (check_built_in(st) == ERROR)
			return (update_exit_code(1, varlist), ERROR);
		if (execute_builtin_or_fork(st, varlist, &lp, *st_head) == ERROR)
			return (update_exit_code(1, varlist), ERROR);
		st = st->next;
	}
	return (wait_process(&lp, varlist, st_head));
}
