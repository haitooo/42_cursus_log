/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_expanded_files.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 00:48:37 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/24 18:36:59 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_wild	*make_new_expanded_files(char *dir)
{
	t_wild	*files;

	files = get_files(".");
	if (files)
		search_same_file(&files, dir);
	files = remove_filename(&files);
	return (files);
}

static t_wild	*add_expanded_files(t_wild **based_files, char *dir)
{
	t_wild	*head;
	t_wild	*files;
	t_wild	*new_files;

	files = NULL;
	head = *based_files;
	while (head)
	{
		new_files = get_files(head->name);
		search_same_file(&new_files, dir);
		new_files = remove_filename(&new_files);
		concat_files(head->name, &new_files);
		if (!files)
			files = new_files;
		else
			connect_to_the_end(&files, &new_files);
		head = head->next;
	}
	return (files);
}

t_wild	*get_expanded_files(char **dirs, t_wild **based_files, int index)
{
	t_wild	*files;

	if (!index)
		files = make_new_expanded_files(dirs[index]);
	else
		files = add_expanded_files(based_files, dirs[index]);
	if (files && dirs[index + 1])
		files = get_expanded_files(dirs, &files, index + 1);
	if (based_files && *based_files)
		free_all_files(based_files);
	return (files);
}
