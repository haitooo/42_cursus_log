/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_wildcard.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 23:42:34 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/24 19:22:02 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*make_new_token(char *first, char *name, int slash)
{
	char	*current;
	char	*new_token;
	int		i;

	i = 0;
	while (!ft_strncmp(&first[i], "./", 2))
		i += 2;
	if (i)
	{
		current = ft_strndup(first, i);
		if (slash)
			new_token = ft_strjoin_three(current, name, "/");
		else
			new_token = ft_strjoin(current, name);
		free(current);
	}
	else if (slash)
		new_token = ft_strjoin(name, "/");
	else
		new_token = ft_strdup(name);
	if (!new_token)
		return (NULL);
	return (new_token);
}

static t_tokens	*replace_first_token(t_tokens **token, char *first, char *name,
		int slash)
{
	t_tokens	*head;

	head = *token;
	head->token = make_new_token(first, name, slash);
	if (!head->token)
		return (NULL);
	head->type = WORD;
	return (head);
}

static t_tokens	*add_new_token(char *first, char *name, int slash)
{
	t_tokens	*new;

	new = (t_tokens *)malloc(sizeof(t_tokens));
	if (!new)
		return (NULL);
	new->token = make_new_token(first, name, slash);
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
	char		*token_head;

	head = *files;
	prev = *first;
	token_head = prev->token;
	if (!replace_first_token(first, token_head, head->name, head->slash))
		return (NULL);
	head = head->next;
	if (!head)
		return (free(token_head), prev);
	while (head)
	{
		new = add_new_token(token_head, head->name, head->slash);
		if (!new)
			return (NULL);
		prev->next = new;
		prev = new;
		head = head->next;
	}
	free(token_head);
	new->next = *next;
	return (new);
}

t_tokens	*open_wildcard(t_tokens **tokens)
{
	t_tokens	*head;
	t_tokens	*next;
	t_wild		*files;
	char		**dirs;

	head = *tokens;
	dirs = split_dir(head->token);
	if (!dirs)
		return (NULL);
	files = get_expanded_files(dirs, NULL, 0);
	free_strs(dirs);
	if (files)
	{
		next = head->next;
		head = add_tokens(&files, &head, &next);
		free_all_files(&files);
	}
	return (head);
}
