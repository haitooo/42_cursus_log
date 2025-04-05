/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 03:25:02 by haito             #+#    #+#             */
/*   Updated: 2025/04/03 15:44:01 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_pipe_wait(t_status *st, t_lp *lp)
{
	int			status;
	t_status	*st_tmp;
	int			sig;

	st_tmp = st;
	while (st_tmp && st_tmp->input_pipefd != -1)
	{
		st_tmp = st_tmp->previous;
		if (st_tmp && st_tmp->pid > 0)
		{
			waitpid(st_tmp->pid, &status, 0);
			if (WIFSIGNALED(status))
			{
				sig = WTERMSIG(status);
				if (sig == SIGINT)
					ft_eprintf("\n");
				lp->result = 128 + sig;
			}
			else
				lp->result = WEXITSTATUS(status);
			lp->count_forked--;
		}
	}
}

void	handle_main_wait(t_status *st, t_lp *lp)
{
	int	status;
	int	sig;
	int	exit_code;

	waitpid(st->pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGQUIT)
			ft_eprintf("Quit (core dumped)\n");
		else if (sig == SIGINT)
			ft_eprintf("\n");
		exit_code = 128 + sig;
	}
	else
		exit_code = WEXITSTATUS(status);
	lp->result = exit_code;
	lp->count_forked--;
}

void	handle_and_or(t_status *st, t_lp *lp, t_var **var)
{
	if (st->next && (st->next->has_and || st->next->has_or
			|| st->next->has_semicolon))
	{
		handle_pipe_wait(st, lp);
		handle_main_wait(st, lp);
	}
	if (st->next && st->next->has_semicolon)
		update_exit_code(lp->result, var);
	update_last_token(st, var);
	g_signal = 0;
}

void	handle_parent_process(t_status *st)
{
	if (st->input_pipefd != -1)
		close(st->input_pipefd);
	if (st->output_pipefd != -1)
		close(st->output_pipefd);
}
