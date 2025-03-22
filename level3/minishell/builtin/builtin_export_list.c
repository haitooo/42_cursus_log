/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_list.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:47:51 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/17 21:34:20 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	print_var(t_var *var)
{
	int		total_len;
	char	*str;

	total_len = ft_strlen(var->name) + ft_strlen(var->value) + 16;
	str = (char *)malloc(sizeof(char) * total_len);
	if (!str)
		return (builtin_error(errno, "malloc error"));
	ft_strlcpy(str, "declare -x ", total_len);
	ft_strlcat(str, var->name, total_len);
	if (var->value)
	{
		ft_strlcat(str, "=\"", total_len);
		ft_strlcat(str, var->value, total_len);
		ft_strlcat(str, "\"", total_len);
	}
	ft_strlcat(str, "\n", total_len);
	ft_putstr_fd(str, STDOUT_FILENO);
	free(str);
	return (SUCCESS);
}

static t_var	*get_first_var(t_var **varlist)
{
	t_var	*head;
	t_var	*first;

	head = *varlist;
	first = *varlist;
	while (head)
	{
		if (ft_strcmp(head->name, first->name) < 0)
			first = head;
		head = head->next;
	}
	return (first);
}

static t_var	*get_next_var(t_var **varlist, t_var *prev)
{
	t_var	*now;
	t_var	*head;

	head = *varlist;
	now = prev;
	while (head && ft_strcmp(now->name, prev->name) <= 0)
	{
		if (ft_strcmp(head->name, now->name) > 0)
			now = head;
		head = head->next;
	}
	while (head)
	{
		if (ft_strcmp(head->name, now->name) < 0 && ft_strcmp(head->name,
				prev->name) > 0)
			now = head;
		head = head->next;
	}
	return (now);
}

int	builtin_export_list(t_var **varlist)
{
	t_var	*prev;
	t_var	*now;

	now = get_first_var(varlist);
	if (ft_strcmp(now->name, "?") && print_var(now) == ERROR)
		return (ERROR);
	while (1)
	{
		prev = now;
		now = get_next_var(varlist, prev);
		if (now == prev)
			break ;
		if (ft_strcmp(now->name, "?") && print_var(now) == ERROR)
			return (ERROR);
	}
	return (SUCCESS);
}
