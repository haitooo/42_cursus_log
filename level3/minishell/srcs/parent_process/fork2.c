/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 19:59:10 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/31 05:10:17 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_child_process(t_status *st, t_var **varlist, t_status *st_head,
		char *heredoc)
{
	t_tokens	*token;
	int			result;
	char		*cmds;
	int			is_builtin;

	if (st->input_pipefd != -1)
		dup2(st->input_pipefd, STDIN_FILENO);
	if (st->output_pipefd != -1)
		dup2(st->output_pipefd, STDOUT_FILENO);
	if (st->has_brackets)
	{
		cmds = st->cmds;
		free_lst_status_(st_head, st->cmds, heredoc);
		result = recursive_continue_line(cmds, varlist, heredoc);
		exit(result);
	}
	token = st->token;
	is_builtin = st->is_builtin;
	free_lst_status(st_head, st);
	if (is_builtin)
		exit(child_call_builtin(&token, varlist, heredoc));
	else
		continue_child(&token, varlist, heredoc);
}

void	fork_process(t_status *st, t_var **varlist,
	t_lp *lp, t_status *st_head)
{
	if (st->has_and_single)
		return ;
	st->pid = fork();
	if (st->pid == -1)
	{
		perror("minishell: fork: ");
		lp->result = FAILED;
		return ;
	}
	lp->count_forked++;
	if (st->pid == 0)
	{
		handle_child_process(st, varlist, st_head, st->heredoc);
	}
	handle_parent_process(st);
	handle_and_or(st, lp, varlist);
}
