/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_edge.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 18:23:38 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/23 21:31:39 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *check_top(t_wild **file, t_words **words, char *name)
{
	char *trimed_name;
	t_wild *target;
	t_words *head;

	target = *file;
	if(!target->flag || !name)
		return NULL;
	trimed_name = ft_strdup(name);
	if(!trimed_name)
		return NULL;
	head= *words;
	while(!ft_strcmp(head->name, "./"))
		head = head->next;
	if(head && head->name[0] == '*')
		return trimed_name;
	if(ft_strncmp(trimed_name, head->name, ft_strlen(head->name)))
	{
		target->flag = 0;
		free(trimed_name);
		return NULL;
	}
	ft_strlcpy(trimed_name, &trimed_name[ft_strlen(head->name)], ft_strlen(&trimed_name[ft_strlen(head->name)]) + 1);
	return trimed_name;
}

char *check_bottom(t_wild **file, t_words **words, char *name)
{
	t_wild *target;
	t_words *head;
	int wlen;
	int nlen;

	target = *file;
	if(!target->flag || !name)
		return NULL;
	head = *words;
	while(head && head->next && head->next->name[0] != '/')
		head = head->next;
	if(head->name[0] == '*')
		return name;
	wlen = ft_strlen(head->name) - 1;
	nlen = ft_strlen(name) - 1;
	if(nlen < 0)
		target->flag = 0;
	while(wlen >= 0 && nlen >= 0)
	{
		if(head->name[wlen] != name[nlen])
		{
			target->flag = 0;
			free(name);
			return NULL;
		}
		wlen--;
		nlen--;
	}
	name[nlen + 1] = '\0';
	return name;
}
