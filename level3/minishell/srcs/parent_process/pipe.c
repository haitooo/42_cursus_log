/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 07:48:56 by haito             #+#    #+#             */
/*   Updated: 2025/03/23 22:24:45 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_operator(const char *cmds)
{
	if (!ft_strcmp(cmds, "||") || !ft_strcmp(cmds, "&&")
		|| !ft_strcmp(cmds, "|") || !ft_strcmp(cmds, "&"))
		return (1);
	return (0);
}

int	process_and_or_operator(t_status **st_head, t_status *st_next, int ope,
		t_var **var)
{
	if (!st_next->next && ope != IS_AND && ope != IS_SEMI)
		return (error_pipe(ERRNO_TWO, var));
	if (ope == IS_SEMI)
	{
		if (st_next->next)
			st_next->next->has_semicolon = 1;
		ft_remove_node(st_head, st_next);
		return (SUCCESS);
	}
	st_next = st_next->next;
	if (ope == IS_OR)
		st_next->has_or = 1;
	if (ope == IS_ANDAND)
		st_next->has_and = 1;
	if (ope == IS_AND)
		st_next->previous->has_and_single = 1;
	ft_remove_node(st_head, st_next->previous);
	return (SUCCESS);
}

int	process_pipe_operator(t_status **st_head, t_status *st, t_status *st_next,
		t_var **var)
{
	int	pipefd[2];

	if (!st_next->next)
		return (error_pipe(ERRNO_TWO, var));
	if (pipe(pipefd) == -1)
		return (error_pipe(ERRNO_THREE, var));
	st->output_pipefd = pipefd[1];
	st_next = st_next->next;
	st_next->input_pipefd = pipefd[0];
	ft_remove_node(st_head, st_next->previous);
	return (SUCCESS);
}

int	process_operator(t_status **st_head, t_status *st, t_var **var)
{
	t_status	*st_next;

	st_next = st->next;
	if (!st_next)
		return (SUCCESS);
	else if (!ft_strcmp(st_next->cmds, "||"))
		return (process_and_or_operator(st_head, st_next, IS_OR, var));
	else if (!ft_strcmp(st_next->cmds, "&&"))
		return (process_and_or_operator(st_head, st_next, IS_ANDAND, var));
	else if (!ft_strcmp(st_next->cmds, "|"))
		return (process_pipe_operator(st_head, st, st_next, var));
	else if (!ft_strcmp(st_next->cmds, "&"))
		return (process_and_or_operator(st_head, st_next, IS_AND, var));
	else if (!ft_strcmp(st_next->cmds, ";"))
		return (process_and_or_operator(st_head, st_next, IS_SEMI, var));
	return (SUCCESS);
}

int	make_pipe(t_status **st_head, t_var **var)
{
	t_status	*st;

	if (!st_head || !*st_head)
		return (ERROR);
	st = *st_head;
	if (is_operator(st->cmds))
		return (error_pipe(ERRNO_ONE, var));
	while (st)
	{
		if (!is_operator(st->cmds))
		{
			if (process_operator(st_head, st, var) == ERROR)
				return (ERROR);
		}
		else
			return (error_pipe(ERRNO_FOUR, var));
		st = st->next;
	}
	return (SUCCESS);
}
