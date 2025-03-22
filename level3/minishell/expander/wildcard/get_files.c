/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_files.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 00:48:16 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/22 00:48:19 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_name_len(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '/')
		i++;
	if (str[i] == '/')
		i++;
	return (i);
}

char	**split_dir(char *str)
{
	int		count;
	char	**dirs;
	char	*copy_str;

	copy_str = str;
	count = 1;
	while (*copy_str)
	{
		copy_str += get_name_len(copy_str);
		count++;
	}
	dirs = (char **)malloc(sizeof(char *) * (count + 1));
	if (!dirs)
		return (NULL);
	count = 0;
	while (*str)
	{
		dirs[count] = ft_strndup(str, get_name_len(str));
		if (!dirs[count])
			return (dirs);
		str += get_name_len(str);
		count++;
	}
	dirs[count] = NULL;
	return (dirs);
}

static t_wild	*convert_dirp(struct dirent **dir_entry, t_wild **prev_file)
{
	t_wild			*new;
	t_wild			*prev;
	struct dirent	*entry;

	entry = *dir_entry;
	new = (t_wild *)malloc(sizeof(t_wild));
	if (!new)
		return (NULL);
	new->name = ft_strdup(entry->d_name);
	if (!new->name)
		return (free(new), NULL);
	new->next = NULL;
	new->flag = 1;
	new->type = entry->d_type;
	prev = *prev_file;
	if (prev)
		prev->next = new;
	return (new);
}

t_wild	*get_files(char *name)
{
	DIR				*dirp;
	struct dirent	*entry;
	t_wild			*start;
	t_wild			*new;
	t_wild			*prev;

	dirp = opendir(name);
	if (!dirp)
		return (NULL);
	start = NULL;
	prev = NULL;
	while (1)
	{
		entry = readdir(dirp);
		if (!entry)
			break ;
		new = convert_dirp(&entry, &prev);
		if (!new)
			return (start);
		if (!start)
			start = new;
		prev = new;
	}
	closedir(dirp);
	return (start);
}
