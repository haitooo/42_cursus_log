/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 17:08:44 by haito             #+#    #+#             */
/*   Updated: 2025/03/27 03:53:38 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_and_or(t_status *st, t_lp *lp, t_var **var)
{
	int			status;
	t_status	*st_tmp;
	int			sig;
	int			exit_code;

	if (st->next && (st->next->has_and || st->next->has_or
			|| st->next->has_semicolon))
	{
		st_tmp = st;
		while (st_tmp->input_pipefd != -1)
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
					lp->result = 128 + WTERMSIG(status);
				}
				else
					lp->result = WEXITSTATUS(status);
				lp->count_forked--;
			}
		}
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
		if (exit_code != 0)
			lp->result = exit_code;

		lp->count_forked--;
	}

	if (st->next && st->next->has_semicolon)
		update_exit_code(lp->result, var);
	g_signal = 0;
}

void	handle_parent_process(t_status *st)
{
	if (st->input_pipefd != -1)
		close(st->input_pipefd);
	if (st->output_pipefd != -1)
		close(st->output_pipefd);
}

void	handle_child_process(t_status *st, t_var **varlist, t_status *st_head)
{
	t_tokens	*token;
	int			result;
	char		*cmds;

	if (st->input_pipefd != -1 && !has_heredoc(st))
		dup2(st->input_pipefd, STDIN_FILENO);
	if (st->output_pipefd != -1 && !has_heredoc(st))
		dup2(st->output_pipefd, STDOUT_FILENO);
	if (st->has_brackets)
	{
		cmds = st->cmds;
		free_lst_status_(st_head, st->cmds);
		result = recursive_continue_line(cmds, varlist);
		exit(result);
	}
	if (st->is_builtin)
	{
		token = st->token;
		free_lst_status(st_head, st);
		exit(child_call_builtin(&token, varlist));
	}
	else
	{
		token = st->token;
		free_lst_status(st_head, st);
		continue_child(&token, varlist, st->output_pipefd);
	}
}
