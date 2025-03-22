/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_same_file.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 21:31:24 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/22 12:03:11 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	isvalid_secret(t_wild **file, char *word, char *name)
{
	t_wild	*head;

	head = *file;
	if (word[0] != '.' && name[0] == '.')
		head->flag = 0;
	return (head->flag);
}

static int	isvalid_wildcard(t_wild **files, char *word, char *name)
{
	t_wild	*head;
	int		i;

	head = *files;
	i = 0;
	while (name[i])
	{
		while (name[i] && name[i] != *word)
			i++;
		if (!ft_strncmp(&name[i], word, ft_strlen(word)))
			return (i + ft_strlen(word));
		if (name[i])
			i++;
		if (!name[i])
			head->flag = 0;
	}
	return (0);
}

static int	isvalid_file(t_wild **files, char *word, char *name)
{
	t_wild	*head;

	head = *files;
	if (ft_strncmp(name, word, ft_strlen(word)))
	{
		head->flag = 0;
		return (0);
	}
	return (ft_strlen(word));
}

static void	manage_flag(t_wild **files, char **words, char *name)
{
	t_wild	*head;
	int		i;

	head = *files;
	i = 0;
	while (words[i] && head->flag)
	{
		if (*words[i] == '\0' && *name)
			head->flag = 0;
		else if (*words[i] != '*' && *words[i] != '/')
			name += isvalid_file(&head, words[i], name);
		else if (*words[i] == '*' && words[i + 1] && *words[i + 1] != '/')
		{
			i++;
			name += isvalid_wildcard(&head, words[i], name);
		}
		else if (*words[i] == '/' && head->type != DT_DIR)
			head->flag = 0;
		i++;
	}
}

void	search_same_file(t_wild **files, char *str)
{
	t_wild	*head;
	char	**words;

	if (!*files)
		return ;
	words = split_wildcards(str, count_wild_words(str));
	if (!words)
		return ;
	head = *files;
	while (head)
	{
		if (isvalid_secret(&head, words[0], head->name))
			manage_flag(&head, words, head->name);
		head = head->next;
	}
	free_strs(words);
}
