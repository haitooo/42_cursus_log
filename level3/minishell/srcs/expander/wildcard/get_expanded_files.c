/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_expanded_files.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 00:48:37 by tssaito           #+#    #+#             */
/*   Updated: 2025/04/03 18:37:28 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_wild	*make_new_expanded_files(t_splited **splited)
{
	t_wild	*files;

	files = get_files(".");
	if (!files)
		return (NULL);
	search_same_file(&files, splited);
	files = remove_filename(&files);
	files = sort_files(&files);
	return (files);
}

static t_wild	*add_expanded_files(t_wild **based_files, t_splited **splited)
{
	t_wild	*head;
	t_wild	*files;
	t_wild	*new_files;

	files = NULL;
	head = *based_files;
	while (head)
	{
		new_files = get_files(head->name);
		if (!new_files)
			break ;
		search_same_file(&new_files, splited);
		new_files = remove_filename(&new_files);
		new_files = sort_files(&new_files);
		concat_files(head->name, &new_files);
		if (!files)
			files = new_files;
		else
			connect_to_the_end(&files, &new_files);
		head = head->next;
	}
	return (files);
}

t_wild	*get_expanded_files(t_splited **splited, t_wild **based_files)
{
	t_wild		*files;
	t_splited	*head;

	if (!based_files)
		files = make_new_expanded_files(splited);
	else
		files = add_expanded_files(based_files, splited);
	head = *splited;
	while (head)
	{
		if (!ft_strcmp(head->str, "/"))
		{
			head = head->next;
			break ;
		}
		head = head->next;
	}
	if (files && head)
		files = get_expanded_files(&head, &files);
	if (based_files && *based_files)
		free_all_files(based_files);
	return (files);
}
