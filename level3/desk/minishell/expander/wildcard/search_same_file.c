/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_same_file.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 21:31:24 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/23 18:59:08 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void free_allwords(t_words **words)
{
	t_words *head;
	t_words *tmp;

	head = *words;
	while(head)
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
	t_words *head;

	target = *file;
	if(name[0] != '.')
		return target->flag;
	head = *words;
	while(head && !ft_strcmp(head->name, "./"))
		head = head->next;
	if (ft_strcmp(head->name, "."))
		target->flag = 0;
	return (target->flag);
}

static void	manage_flag(t_wild **files, t_words **words, char *name)
{
	t_wild *target;
	t_words *head;
	char *saved;

	head = *words;
	target = *files;
	if(!target->flag || !name || !head)
		return ;
	if(head->type == WILD)
		head = head->next;
	if(head && (head->name[0] == '/' || (head->next && head->next->name[0] == '/')))
	{
		if(target->type != DT_DIR)
			target->flag = 0;
		else
			target->slush = 1;
		return;
	}
	while(head && head->type != SLUSH && head->next && target->flag)
	{
		while(*name && ft_strncmp(name, head->name, ft_strlen(head->name)))
			name++;
		if(!*name)
			target->flag = 0;
		else
		{
			saved = name;
			name += ft_strlen(head->name);
			manage_flag(files, &(head->next), name);
			if(target->flag)
				break;
			else
				target->flag = 1;
			name = saved + 1;
		}
	}
}

void	search_same_file(t_wild **files, char *str)
{
	t_wild	*head;
	t_words	*words;
	t_words	*saved_words;
	char	*trimed_name;

	if (!*files)
		return ;
	saved_words = split_wildcards(str);
	if (!saved_words)
		return ;
	head = *files;
	while (head)
	{
		words = saved_words;
		if (isvalid_secret(&head, &words, head->name))
		{
			trimed_name =  check_top(&head, &words, head->name);
			trimed_name =  check_bottom(&head, &words, trimed_name);
			while(words && !ft_strcmp(words->name, "./"))
				words = words->next;
			if(words && words->name[0] != '*')
				words = words->next;
			if(words && words->type == ELSE && words->name[0] == '*')
				words = words->next;
			manage_flag(&head, &words, trimed_name);
			free(trimed_name);
		}
		head = head->next;
	}
	free_allwords(&saved_words);
}
