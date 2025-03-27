/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 19:02:05 by haito             #+#    #+#             */
/*   Updated: 2025/03/26 16:03:11 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_success_exit(t_lp *lp, t_status *st, t_var **varlist)
{
	t_tokens	*token;
	char		*name_dup;
	char		*value_dup;

	while (st)
	{
		if (st->pid == lp->last_pid)
			break ;
		st = st->next;
	}
	if (!st || !st->token)
		return ;
	token = st->token;
	while (token->next)
		token = token->next;
	name_dup = ft_strdup("_");
	if (!name_dup)
		error_node(ERRNO_ONE);
	value_dup = ft_strdup(token->token);
	if (!value_dup)
		error_node(ERRNO_ONE);
	add_var(varlist, name_dup, value_dup);
}

static int	wait_for_last_pid(t_lp *lp, t_status *st, t_var **varlist)
{
	int		status;
	int		exit_code;
	int		sig;
	pid_t	pid;

	pid = waitpid(lp->last_pid, &status, 0);
	if (pid == -1)
		return (perror("minishell: waitpid"), FAILED);
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGQUIT)
			ft_eprintf("Quit (core dumped)\n");
		exit_code = 128 + sig;
	}
	else
		exit_code = WEXITSTATUS(status);
	if (exit_code == SUCCESS)
		handle_success_exit(lp, st, varlist);
	return (exit_code);
}

static void	wait_for_all_processes(t_lp *lp, t_status *st)
{
	int	wait_count;

	wait_count = 0;
	while (st && wait_count < lp->count_forked)
	{
		if (st->pid > 0)
		{
			waitpid(st->pid, NULL, 0);
			if (g_signal == SIGINT)
				ft_eprintf("\n");
			g_signal = 0;
			wait_count++;
		}
		st = st->next;
	}
}

int	wait_process(t_lp *lp, t_var **varlist, t_status **st_head)
{
	int			exit_code;
	t_status	*st;
	t_tokens	*token;
	char		*name_dup;
	char		*value_dup;

	st = *st_head;
	if (lp->count_forked != 0)
		exit_code = wait_for_last_pid(lp, st, varlist);
	else
	{
		exit_code = lp->result;
		if (!st || !st->token)
			return (update_exit_code(exit_code, varlist));
		token = st->token;
		while (token->next)
			token = token->next;
		name_dup = ft_strdup("_");
		if (!name_dup)
			error_node(ERRNO_ONE);
		value_dup = ft_strdup(token->token);
		if (!value_dup)
			error_node(ERRNO_ONE);
		add_var(varlist, name_dup, value_dup);
	}
	wait_for_all_processes(lp, st);
	g_signal = 0;
	return (update_exit_code(exit_code, varlist));
}

