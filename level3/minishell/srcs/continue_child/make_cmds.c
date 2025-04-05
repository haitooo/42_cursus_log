/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 22:00:45 by tssaito           #+#    #+#             */
/*   Updated: 2025/04/03 16:45:15 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_cmds(t_tokens **tokens)
{
	int			count;
	t_tokens	*head;

	head = *tokens;
	count = 0;
	while (head)
	{
		if (head->type != WORD && head->type != HAVE_QUOTE)
			head = head->next->next;
		else if (head->token)
		{
			count++;
			head = head->next;
		}
		else
			head = head->next;
	}
	return (count);
}

static char	**copy_cmds(t_child *child, t_tokens **tokens, int malloc_size)
{
	int			i;
	char		**cmds;
	t_tokens	*head;

	cmds = (char **)malloc(sizeof(char *) * (malloc_size + 1));
	if (!cmds)
		exit_child(child, EXIT_FAILURE, errno, "malloc error");
	i = 0;
	head = *tokens;
	while (cmds && head)
	{
		if (head->type != WORD && head->type != HAVE_QUOTE)
			head = head->next->next;
		else if (head->token)
		{
			cmds[i] = head->token;
			i++;
			head = head->next;
		}
		else
			head = head->next;
	}
	if (cmds)
		cmds[i] = NULL;
	return (cmds);
}

void	make_cmds(t_child *child, t_tokens **tokens)
{
	int	malloc_size;

	malloc_size = count_cmds(child->tokens);
	child->cmds = copy_cmds(child, tokens, malloc_size);
	if (!*child->cmds)
		exit_child(child, EXIT_SUCCESS, errno, NULL);
	if (!ft_strcmp(child->cmds[0], "."))
		exit_child(child, EXIT_SYNTAX, FILENUMERROR, ".");
}
