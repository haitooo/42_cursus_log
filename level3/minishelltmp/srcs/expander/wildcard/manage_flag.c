/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_flag.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 23:42:34 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/24 18:24:53 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_end_with_slash(t_words **words, t_wild **files)
{
	t_wild	*target;
	t_words	*head;

	head = *words;
	target = *files;
	if (head->name[0] == '/' || (head->next && head->next->name[0] == '/'))
	{
		if (target->type != DT_DIR)
			target->flag = 0;
		else
			target->slash = 1;
		return (1);
	}
	return (0);
}

void	manage_flag(t_wild **files, t_words **words, char *name)
{
	t_wild	*target;
	t_words	*head;

	head = *words;
	target = *files;
	if (head && head->type == WILD)
		head = head->next;
	if (!target->flag || !name || !head || is_end_with_slash(&head, &target))
		return ;
	while (head && head->type != SLASH && head->next && target->flag)
	{
		while (*name && ft_strncmp(name, head->name, ft_strlen(head->name)))
			name++;
		if (!*name)
			target->flag = 0;
		else
		{
			manage_flag(files, &(head->next), name + ft_strlen(head->name));
			if (target->flag)
				break ;
			else
				target->flag = 1;
			name++;
		}
	}
}
