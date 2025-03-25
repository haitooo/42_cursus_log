/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 01:21:03 by haito             #+#    #+#             */
/*   Updated: 2025/03/24 21:09:35 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	update_exit_code(int exit_code, t_var **varlist)
{
	t_var	*var;

	if (!varlist || !*varlist)
		return (ft_eprintf("minishell: not found exit status\n"), ERROR);
	var = *varlist;
	while (var && ft_strcmp(var->name, "?") != 0)
		var = var->next;
	if (!var)
		return (ft_eprintf("minishell: not found exit status\n"), ERROR);
	free(var->value);
	var->value = ft_itoa(exit_code);
	if (!var->value)
		return (error_node(ERRNO_ONE));
	return (SUCCESS);
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
		handle_child_process(st, varlist, st_head);
	handle_parent_process(st);
	handle_and_or(st, lp, varlist);
}

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
		if (g_signal == SIGINT)
		{
			write(STDOUT_FILENO, "\n", 1);
			g_signal = 0;
			lp.result = 130;
			lp.count_forked = 0;
			break ;
		}
		if (st->token)
			free_tokens(&(st->token));
		st->token = expander(st->cmds, varlist);
		if (!st->token)
			return (update_exit_code(1, varlist), ERROR);
		if (check_built_in(st) == ERROR)
			return (update_exit_code(1, varlist), ERROR);
		if ((st->has_and && lp.result != 0) || (st->has_or && lp.result == 0))
			;
		else if (is_direct_builtin(st))
			lp.result = call_builtin(&st->token, varlist, *st_head);
		else
		{
			fork_process(st, varlist, &lp, *st_head);
			if (!st->next || (!st->next->has_and && !st->next->has_or))
				lp.last_pid = st->pid;
		}
		st = st->next;
	}
	return (wait_process(&lp, varlist, st_head));
}
