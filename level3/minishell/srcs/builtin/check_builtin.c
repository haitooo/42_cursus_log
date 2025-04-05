/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 02:13:25 by haito             #+#    #+#             */
/*   Updated: 2025/03/31 11:34:15 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tokens	*get_cmd_token(t_tokens *head)
{
	while (head && (head->type >= TRUNC && head->type <= HEREDOC))
	{
		if (head->next && head->next->next)
			head = head->next->next;
		else
			break ;
	}
	return (head);
}

int	check_built_in(t_status *st)
{
	char		**builtin_cmds;
	int			i;
	t_tokens	*head;

	if (!st)
		return (ERROR);
	if (!st->token)
		return (SUCCESS);
	builtin_cmds = init_builtin_cmds();
	if (!builtin_cmds)
		return (error_node(ERRNO_ONE), ERROR);
	i = -1;
	head = get_cmd_token(st->token);
	while (head && builtin_cmds[++i])
	{
		if (!ft_strcmp(head->token, builtin_cmds[i]))
		{
			st->is_builtin = 1;
			break ;
		}
	}
	free_builtin_cmds(builtin_cmds);
	return (SUCCESS);
}
