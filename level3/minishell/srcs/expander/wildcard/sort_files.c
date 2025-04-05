/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 18:07:33 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/30 13:09:32 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_samechars(char *prev, char *next)
{
	int	i;

	i = 0;
	while (prev[i] && next[i])
	{
		if (prev[i] == next[i] || prev[i] - next[i] == 32 || prev[i]
			- next[i] == -32)
			i++;
		else
		{
			if (ft_islower(prev[i]) && ft_isupper(next[i]))
				return (prev[i] - 32 - next[i]);
			if (ft_isupper(prev[i]) && ft_islower(next[i]))
				return (prev[i] + 32 - next[i]);
			return (prev[i] - next[i]);
		}
	}
	if (prev[i] || next[i])
		return (prev[i] - next[i]);
	return (-1 * (prev[0] - next[0]));
}

static int	filename_cmp(char *prev, char *next)
{
	int	i;

	i = 0;
	while (prev[i] && next[i])
	{
		if (prev[i] == next[i])
			i++;
		else if (prev[i] - next[i] == 32 || prev[i] - next[i] == -32)
			return (check_samechars(&prev[i], &next[i]));
		else
		{
			if (ft_islower(prev[i]) && ft_isupper(next[i]))
				return (prev[i] - 32 - next[i]);
			if (ft_isupper(prev[i]) && ft_islower(next[i]))
				return (prev[i] + 32 - next[i]);
			return (prev[i] - next[i]);
		}
	}
	return (prev[i] - next[i]);
}

t_wild	*issorted_files(t_wild **files, int *flag)
{
	t_wild	*head;
	t_wild	*start;
	t_wild	*prev;

	start = *files;
	head = *files;
	while (head && head->next)
	{
		if (filename_cmp(head->name, head->next->name) > 0)
		{
			prev = start;
			while (prev != head && prev->next != head)
				prev = prev->next;
			if (head == start)
				start = head->next;
			prev->next = head->next;
			prev = head->next;
			head->next = head->next->next;
			prev->next = head;
			*flag = 1;
		}
		else
			head = head->next;
	}
	return (start);
}

t_wild	*sort_files(t_wild **files)
{
	t_wild	*head;
	t_wild	*start;
	int		flag;

	start = *files;
	flag = 1;
	while (flag)
	{
		flag = 0;
		head = start;
		start = issorted_files(&head, &flag);
	}
	return (start);
}
