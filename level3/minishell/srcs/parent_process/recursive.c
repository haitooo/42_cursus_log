/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recursive.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 22:01:42 by haito             #+#    #+#             */
/*   Updated: 2025/03/31 05:25:03 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	fork_process_(t_status *st, t_var **varlist,
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
		free(lp->input);
		handle_child_process(st, varlist, st_head, lp->heredoc);
	}
	handle_parent_process(st);
	handle_and_or(st, lp, varlist);
}

int	has_heredoc(t_status *st)
{
	t_tokens	*token;

	token = st->token;
	while (token)
	{
		if (token->type == HEREDOC && token->next)
			return (1);
		token = token->next;
	}
	return (0);
}

void	execute_builtin_or_fork_(t_status *st, t_var **varlist, t_lp *lp,
	t_status *st_head)
{
	lp->heredoc_tmp = NULL;
	if (is_direct_builtin(st))
	{
		if (lp->heredoc)
		{
			lp->heredoc_tmp = ft_strdup(lp->heredoc);
			if (!lp->heredoc_tmp)
			{
				error_node(ERRNO_ONE);
				exit(1);
			}
		}
		lp->result = call_builtin_re(&st->token, varlist, st_head,
				lp);
	}
	else
	{
		fork_process_(st, varlist, lp, st_head);
		if (!st->next || (!st->next->has_and && !st->next->has_or))
			lp->last_pid = st->pid;
	}
}

int	fork_and_wait_(t_status **st_head, t_var **varlist,
		char *input, char *heredoc)
{
	t_status	*st;
	t_lp		lp;

	st = *st_head;
	lp.count_forked = 0;
	lp.last_pid = -1;
	lp.result = 0;
	lp.input = input;
	lp.heredoc = heredoc;
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
		execute_builtin_or_fork_(st, varlist, &lp, *st_head);
		st = st->next;
	}
	return (wait_process(&lp, varlist, st_head));
}

int	recursive_continue_line(char *input, t_var **varlist, char *heredoc)
{
	t_status	*state;
	t_brackets	brackets;
	t_var		*exit_var;
	int			ret;

	if (!input)
		return (0);
	if (find_brackets_pair(input, &brackets, ft_strlen(input),
			varlist) == ERROR)
		return (free_varlist(varlist), free(input), free(heredoc), ERROR);
	state = sep_input_to_cmds(input, &brackets, NULL, varlist);
	if (!state)
		return (free_varlist(varlist), free(input), free(heredoc), ERROR);
	if (make_pipe(&state, varlist) == ERROR)
		return (frees(state, varlist), free(input), free(heredoc), ERROR);
	if (fork_and_wait_(&state, varlist, input, heredoc) == ERROR)
		return (frees(state, varlist), free(input), free(heredoc), ERROR);
	exit_var = get_var(varlist, "?");
	ret = ft_atoi_exit(exit_var->value);
	return (free(input), free(heredoc), frees(state, varlist), ret);
}
