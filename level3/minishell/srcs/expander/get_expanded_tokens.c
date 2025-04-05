/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_expanded_tokens.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 13:43:06 by tssaito           #+#    #+#             */
/*   Updated: 2025/04/03 15:34:17 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	has_char(char *str, char key)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == key)
			return (1);
		i++;
	}
	return (0);
}

static t_splited	*get_saved_point(t_splited **splited)
{
	t_splited	*head;
	int			i;

	head = *splited;
	while (head)
	{
		if (head->str && head->type == PLAINVAR && has_char(head->str, ' '))
		{
			i = 0;
			while (head->str[i] && head->str[i] != ' ')
			{
				if (head->str[i] == '=')
					return (NULL);
				i++;
			}
			break ;
		}
		if (head->str && has_char(head->str, '='))
			return (NULL);
		head = head->next;
	}
	return (head);
}

static t_splited	*split_with_space(t_splited **splited)
{
	t_splited	*head;
	t_splited	*saved;
	int			i;

	head = get_saved_point(splited);
	if (!head || !head->str)
		return (NULL);
	saved = (t_splited *)malloc(sizeof(t_splited));
	if (!saved)
		return (NULL);
	saved->str = ft_strdup(head->str);
	saved->next = head->next;
	saved->type = head->type;
	head->next = NULL;
	if (!saved->str)
		return (free(saved), NULL);
	i = 0;
	while (saved->str[i] && !ft_isspace(saved->str[i]))
		i++;
	head->str[i] = '\0';
	while (ft_isspace(saved->str[i]))
		i++;
	ft_strlcpy(saved->str, &saved->str[i], ft_strlen(&saved->str[i]) + 1);
	return (saved);
}

t_tokens	*get_expanded_tokens(t_splited **splited)
{
	t_splited	*head;
	t_splited	*saved;
	t_tokens	*new;
	t_tokens	*prev;
	t_tokens	*start;

	saved = *splited;
	start = NULL;
	while (saved)
	{
		head = saved;
		saved = split_with_space(&head);
		new = check_wildcard(&head);
		if (!new)
			return (free_tokens(&start), free_splited(&saved), NULL);
		if (!start)
			start = new;
		else
			prev->next = new;
		prev = new;
		while (prev && prev->next)
			prev = prev->next;
	}
	return (start);
}
