/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_free.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 20:58:59 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/21 21:30:21 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_file(t_wild **file)
{
	t_wild	*head;

	head = *file;
	free(head->name);
	free(head);
}

void	free_all_files(t_wild **files)
{
	t_wild	*head;
	t_wild	*tmp;

	head = *files;
	while (head)
	{
		tmp = head;
		head = head->next;
		free_file(&tmp);
	}
}

static int	is_invalid_name(char *str)
{
	if (!ft_strcmp(str, ".") || !ft_strcmp(str, ".."))
		return (ERROR);
	return (SUCCESS);
}

static t_wild	*remove_top_filename(t_wild **files)
{
	t_wild	*start;
	t_wild	*tmp;

	if (!*files)
		return (NULL);
	start = *files;
	while (start && (!start->flag || is_invalid_name(start->name) == ERROR))
	{
		tmp = start;
		start = start->next;
		free_file(&tmp);
	}
	return (start);
}

t_wild	*remove_filename(t_wild **files)
{
	t_wild	*start;
	t_wild	*head;
	t_wild	*prev;
	t_wild	*tmp;

	start = remove_top_filename(files);
	head = start;
	prev = start;
	while (head)
	{
		if (!head->flag || is_invalid_name(head->name) == ERROR)
		{
			prev->next = head->next;
			tmp = head;
			head = head->next;
			free_file(&tmp);
		}
		else
		{
			prev = head;
			head = head->next;
		}
	}
	return (start);
}
