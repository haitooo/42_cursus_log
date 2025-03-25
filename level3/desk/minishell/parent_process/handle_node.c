/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_node.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 02:24:14 by haito             #+#    #+#             */
/*   Updated: 2025/03/23 21:41:31 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_status	*ft_new_node(const char *cmds, int has_brackets)
{
	t_status	*new_node;

	if (!cmds)
		return (NULL);
	new_node = (t_status *)malloc(sizeof(t_status));
	if (!new_node)
		return (NULL);
	new_node->cmds = ft_strdup(cmds);
	if (!new_node->cmds)
		return (NULL);
	new_node->token = NULL;
	new_node->pid = 0;
	new_node->input_pipefd = -1;
	new_node->output_pipefd = -1;
	new_node->has_brackets = has_brackets;
	new_node->has_or = 0;
	new_node->has_and = 0;
	new_node->has_and_single = 0;
	new_node->has_semicolon = 0;
	new_node->is_builtin = 0;
	new_node->previous = NULL;
	new_node->next = NULL;
	return (new_node);
}

void	ft_add_back_node(t_status **head, t_status *new_node)
{
	t_status	*tmp;

	if (!head || !new_node)
		return ;
	if (*head == NULL)
	{
		*head = new_node;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
	new_node->previous = tmp;
}

void	ft_remove_node(t_status **head, t_status *node)
{
	if (!head || !*head || !node)
		return ;
	if (*head == node)
		*head = node->next;
	if (node->previous)
		node->previous->next = node->next;
	if (node->next)
		node->next->previous = node->previous;
	free(node->cmds);
	free(node);
}

int	add_command_node(char **cmds, t_status **st_head, t_var **var)
{
	t_status	*new_node;

	if (*cmds)
	{
		*cmds = trim_spaces(*cmds);
		if (!(*cmds))
			return (update_exit_code(FAILED, var), ERROR);
		new_node = ft_new_node(*cmds, 0);
		if (!new_node)
			return (update_exit_code(FAILED, var), ERROR);
		ft_add_back_node(st_head, new_node);
		free(*cmds);
		*cmds = NULL;
	}
	return (0);
}

int	add_operator_node(const char *op, char **cmds, t_status **st_head,
		t_parser *ps)
{
	t_status	*new_node;

	if (add_command_node(cmds, st_head, ps->var) == ERROR)
		return (update_exit_code(FAILED, ps->var), error_node(ERRNO_ONE));
	new_node = ft_new_node(op, 0);
	if (!new_node)
		return (update_exit_code(FAILED, ps->var), error_node(ERRNO_ONE));
	ft_add_back_node(st_head, new_node);
	return (ps->i + (ft_strlen(op)));
}
