/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_flag.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 23:42:34 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/30 13:09:24 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_end_with_slash(t_splited **splited, t_wild **files)
{
	t_wild		*target;
	t_splited	*head;

	head = *splited;
	target = *files;
	if (head->str[0] == '/' || (head->next && head->next->str[0] == '/'))
	{
		if (target->type != DT_DIR)
			target->flag = 0;
		else
			target->slash = 1;
		return (1);
	}
	return (0);
}

void	manage_flag(t_wild **files, t_splited **splited, char *name)
{
	t_wild		*target;
	t_splited	*head;

	head = *splited;
	target = *files;
	if (head && head->type == WILDS)
		head = head->next;
	if (!target->flag || !name || !head || is_end_with_slash(&head, &target))
		return ;
	while (head && ft_strcmp(head->str, "/") && head->next && target->flag)
	{
		while (*name && ft_strncmp(name, head->str, ft_strlen(head->str)))
			name++;
		if (!*name)
			target->flag = 0;
		else
		{
			manage_flag(files, &(head->next), name + ft_strlen(head->str));
			if (target->flag)
				break ;
			else
				target->flag = 1;
			name++;
		}
	}
}
