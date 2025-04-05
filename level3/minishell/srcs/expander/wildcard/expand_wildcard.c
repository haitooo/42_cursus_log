/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 01:03:02 by tssaito           #+#    #+#             */
/*   Updated: 2025/04/03 17:54:36 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_current_str(t_splited **splited)
{
	int			i;
	int			count;
	t_splited	*head;
	char		*current_str;

	head = *splited;
	count = 0;
	while (head && !ft_strcmp(head->str, ".") && head->next
		&& !ft_strcmp(head->next->str, "/"))
	{
		count++;
		head = head->next->next;
	}
	if (!count)
		return (NULL);
	current_str = (char *)malloc(sizeof(char) * count * 2 + 1);
	if (!current_str)
		return (NULL);
	i = 0;
	current_str[0] = '\0';
	while (i++ < count)
		ft_strlcat(current_str, "./", count * 2 + 1);
	return (current_str);
}

static char	*make_new_token(char *current, char *name, int slash)
{
	char	*new_token;

	if (current)
	{
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

static t_tokens	*add_new_token(t_wild **files, t_splited **splited)
{
	t_tokens	*new;
	char		*current_str;
	t_splited	*head;
	int			slash;

	new = (t_tokens *)malloc(sizeof(t_tokens));
	if (!new)
		return (NULL);
	current_str = get_current_str(splited);
	head = *splited;
	slash = 0;
	while (head)
	{
		if (head->next && head->next->str[0] == '/' && !head->next->next)
			slash = 1;
		head = head->next;
	}
	new->token = make_new_token(current_str, (*files)->name, slash);
	if (!new->token)
		return (free(new), NULL);
	new->type = WORD;
	new->next = NULL;
	return (new);
}

static t_tokens	*add_tokens(t_wild **files, t_splited **splited)
{
	t_wild		*head;
	t_tokens	*new;
	t_tokens	*prev;
	t_tokens	*start;

	head = *files;
	start = NULL;
	while (head)
	{
		new = add_new_token(&head, splited);
		if (!new)
			return (free_tokens(&start), NULL);
		if (!start)
			start = new;
		else
			prev->next = new;
		prev = new;
		head = head->next;
	}
	return (start);
}

t_tokens	*expand_wildcard(t_splited **splited)
{
	t_tokens	*new;
	t_wild		*files;
	t_splited	*head;

	head = *splited;
	while (head && !ft_strcmp(head->str, ".") && head->next
		&& !ft_strcmp(head->next->str, "/"))
		head = head->next->next;
	files = get_expanded_files(&head, NULL);
	new = add_tokens(&files, splited);
	free_all_files(&files);
	return (new);
}
