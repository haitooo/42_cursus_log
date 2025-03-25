/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 01:03:02 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/23 20:00:59 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_tokens	*replace_first_token(t_tokens **token, char *first, char *name, int slush)
{
	t_tokens	*head;
	char *current;
	int i;

	head = *token;
	i = 0;
	while(!ft_strncmp(&first[i], "./", 2))
		i += 2;
	if(i)
	{
		current = ft_strndup(first, i);
		if(slush)
			head->token = ft_strjoin_three(current, name, "/");
		else
			head->token = ft_strjoin(current, name);
		free(current);
	}
	else if(slush)
		head->token = ft_strjoin(name, "/");
	else
		head->token = ft_strdup(name);
	if (!head->token)
		return (NULL);
	head->type = WORD;
	return (head);
}

static t_tokens	*get_new_token(char *first, char *name, int slush)
{
	t_tokens	*new;
	char *current;
	int i;

	new = (t_tokens *)malloc(sizeof(t_tokens));
	if (!new)
		return (NULL);
	i = 0;
	while(!ft_strncmp(&first[i], "./", 2))
		i += 2;
	if(i)
	{
		current = ft_strndup(first, i);
		if(slush)
			new->token = ft_strjoin_three(current, name, "/");
		else
			new->token = ft_strjoin(current, name);
		free(current);
	}
	else if(slush)
		new->token = ft_strjoin(name, "/");
	else
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
	char *token_head;

	head = *files;
	prev = *first;
	token_head = prev->token;
	if (!replace_first_token(first, token_head, head->name, head->slush))
		return (NULL);
	head = head->next;
	if (!head)
		return (prev);
	while (head)
	{
		new = get_new_token(token_head, head->name, head->slush);
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

static int isvalid_type(t_type prev, char *str)
{
	if(prev != APPEND && prev != TRUNC && prev != INPUT && prev != HEREDOC)
		return SUCCESS;
	if(ft_strcmp(str, "*"))
		return SUCCESS;
	return ERROR;
}

void	expand_wildcard(t_tokens **tokens)
{
	t_tokens	*head;
	t_tokens	*next;
	t_wild		*files;
	char		**dirs;
	t_type		prev_type;

	head = *tokens;
	prev_type = WORD;
	while (head)
	{
		if (isvalid_type(prev_type, head->token) != ERROR && check_wildcard(head->token))
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
		{
			prev_type = head->type;
			head = head->next;
		}
	}
}
