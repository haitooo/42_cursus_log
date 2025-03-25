/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 17:08:44 by haito             #+#    #+#             */
/*   Updated: 2025/03/24 12:07:29 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_and_or(t_status *st, t_lp *lp, t_var **var)
{
	int	status;

	if (st->next && (st->next->has_and || st->next->has_or
			|| st->next->has_semicolon))
	{
		waitpid(st->pid, &status, 0);
		lp->result = WEXITSTATUS(status);
		lp->count_forked--;
	}
	if (st->next && st->next->has_semicolon)
		update_exit_code(lp->result, var);
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

	if (st->input_pipefd != -1)
		dup2(st->input_pipefd, STDIN_FILENO);
	if (st->output_pipefd != -1)
		dup2(st->output_pipefd, STDOUT_FILENO);
	if (st->has_brackets)
	{
		free_lst_status_(st_head, st->cmds);
		result = recursive_continue_line(st->cmds, varlist);
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
		continue_child(&token, varlist);
	}
}
