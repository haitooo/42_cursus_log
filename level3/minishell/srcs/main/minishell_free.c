/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_free.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 19:42:18 by haito             #+#    #+#             */
/*   Updated: 2025/03/31 05:11:16 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_builtin_cmds(char **builtin_cmds)
{
	int	i;

	i = 0;
	while (builtin_cmds[i])
		free(builtin_cmds[i++]);
	free(builtin_cmds);
}

void	free_lst_status(t_status *st_head, t_status *st)
{
	t_status	*current;
	t_status	*next_node;

	if (!st_head)
		return ;
	current = st_head;
	while (current)
	{
		next_node = current->next;
		if (current->cmds)
			free(current->cmds);
		if (current->token && (!st || (st && st != current)))
			free_tokens(&current->token);
		if (current->input_pipefd != -1)
			close(current->input_pipefd);
		if (current->output_pipefd != -1)
			close(current->output_pipefd);
		if (current->heredoc && (!st || (st && st != current)))
			free(current->heredoc);
		free(current);
		current = next_node;
	}
}

void	free_lst_status_(t_status *st_head, char *cmds, char *heredoc)
{
	t_status	*current;
	t_status	*next_node;

	if (!st_head)
		return ;
	current = st_head;
	while (current)
	{
		next_node = current->next;
		if (current->cmds && current->cmds != cmds)
			free(current->cmds);
		if (current->token)
			free_tokens(&current->token);
		if (current->input_pipefd != -1)
			close(current->input_pipefd);
		if (current->output_pipefd != -1)
			close(current->output_pipefd);
		if (current->heredoc && current->heredoc != heredoc)
			free(current->heredoc);
		free(current);
		current = next_node;
	}
}

void	frees(t_status *st_head, t_var **varlist)
{
	free_lst_status(st_head, NULL);
	free_varlist(varlist);
}
