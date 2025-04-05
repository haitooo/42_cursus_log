/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_list.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 15:47:51 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/30 17:50:00 by tssaito          ###   ########.fr       */
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
	t_var	*next;
	t_var	*head;

	head = *varlist;
	next = prev;
	while (head && ft_strcmp(next->name, prev->name) <= 0)
	{
		if (ft_strcmp(head->name, next->name) > 0)
			next = head;
		head = head->next;
	}
	while (head)
	{
		if (ft_strcmp(head->name, next->name) < 0 && ft_strcmp(head->name,
				prev->name) > 0)
			next = head;
		head = head->next;
	}
	return (next);
}

int	is_printable_var(char *name)
{
	if (!ft_strcmp(name, "_") || !ft_strcmp(name, "?"))
		return (0);
	return (1);
}

int	builtin_export_list(t_var **varlist)
{
	t_var	*prev;
	t_var	*now;

	now = get_first_var(varlist);
	if (is_printable_var(now->name) && print_var(now) == ERROR)
		return (ERROR);
	while (1)
	{
		prev = now;
		now = get_next_var(varlist, prev);
		if (now == prev)
			break ;
		if (is_printable_var(now->name) && print_var(now) == ERROR)
			return (ERROR);
	}
	return (SUCCESS);
}
