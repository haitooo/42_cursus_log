/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_same_file.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 21:31:24 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/24 18:36:54 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_allwords(t_words **words)
{
	t_words	*head;
	t_words	*tmp;

	head = *words;
	while (head)
	{
		tmp = head;
		head = head->next;
		free(tmp->name);
		free(tmp);
	}
}

static int	isvalid_secret(t_wild **file, t_words **words, char *name)
{
	t_wild	*target;
	t_words	*head;

	target = *file;
	if (name[0] != '.')
		return (target->flag);
	head = *words;
	while (head && !ft_strcmp(head->name, "./"))
		head = head->next;
	if (ft_strcmp(head->name, "."))
		target->flag = 0;
	return (target->flag);
}

static t_words	*move_words_start(t_words **words)
{
	t_words	*head;

	head = *words;
	while (head && !ft_strcmp(head->name, "./"))
		head = head->next;
	if (head && head->name[0] != '*')
		head = head->next;
	if (head && head->type == ELSE && head->name[0] == '*')
		head = head->next;
	return (head);
}

void	search_same_file(t_wild **files, char *str)
{
	t_wild	*head;
	t_words	*words;
	t_words	*saved_words;
	char	*saved_trimed_name;

	saved_words = split_wildcards(str);
	if (!saved_words)
		return ;
	head = *files;
	while (head)
	{
		words = saved_words;
		if (isvalid_secret(&head, &words, head->name))
		{
			saved_trimed_name = trim_word_start(&head, &words, head->name);
			trim_word_end(&head, &words, saved_trimed_name);
			words = move_words_start(&words);
			manage_flag(&head, &words, saved_trimed_name);
			free(saved_trimed_name);
		}
		head = head->next;
	}
	free_allwords(&saved_words);
}
