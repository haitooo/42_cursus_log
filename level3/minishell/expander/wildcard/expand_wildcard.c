/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 01:03:02 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/22 01:03:03 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_tokens	*replace_first_token(t_tokens **first, char *name)
{
	t_tokens	*head;

	head = *first;
	free(head->token);
	head->token = ft_strdup(name);
	if (!head->token)
		return (NULL);
	head->type = WORD;
	return (head);
}

static t_tokens	*get_new_token(char *name)
{
	t_tokens	*new;

	new = (t_tokens *)malloc(sizeof(t_tokens));
	if (!new)
		return (NULL);
	new->token = ft_strdup(name);
	if (!new->token)
		return (free(new), NULL);
	new->type = WORD;
	new->next = NULL;
	return (new);
}

static t_tokens	*add_tokens(t_wild **files, t_tokens **first, t_tokens **next)
{
	t_wild		*head;
	t_tokens	*new;
	t_tokens	*prev;

	head = *files;
	if (!replace_first_token(first, head->name))
		return (NULL);
	head = head->next;
	prev = *first;
	if (!head)
		return (prev);
	while (head)
	{
		new = get_new_token(head->name);
		if (!new)
			return (NULL);
		prev->next = new;
		prev = new;
		head = head->next;
	}
	new->next = *next;
	return (new);
}

void	expand_wildcard(t_tokens **tokens)
{
	t_tokens	*head;
	t_tokens	*next;
	t_wild		*files;
	char		**dirs;

	head = *tokens;
	while (head)
	{
		if (check_wildcard(head->token))
		{
			dirs = split_dir(head->token);
			if (!dirs)
				return ;
			files = get_expanded_files(dirs, NULL, 0);
			free_strs(dirs);
			if (files)
			{
				next = head->next;
				head = add_tokens(&files, &head, &next);
				free_all_files(&files);
			}
		}
		if (head)
			head = head->next;
	}
}
