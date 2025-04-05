/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_same_file.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 21:31:24 by tssaito           #+#    #+#             */
/*   Updated: 2025/04/03 17:37:37 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	is_secret(t_wild **file, t_splited **splited, char *name)
{
	t_wild		*head;
	t_splited	*start;

	if (name[0] != '.')
		return ;
	start = *splited;
	head = *file;
	if (start->str[0] != '.')
		head->flag = 0;
}

void	search_same_file(t_wild **files, t_splited **splited)
{
	t_wild		*head;
	t_splited	*start;
	char		*saved_trimed_name;

	head = *files;
	while (head)
	{
		is_secret(&head, splited, head->name);
		saved_trimed_name = trim_word_start(&head, splited, head->name);
		trim_word_end(&head, splited, saved_trimed_name);
		start = *splited;
		if (start->type != WILDS)
			start = start->next;
		if (start->type == WILDS)
			start = start->next;
		manage_flag(&head, &start, saved_trimed_name);
		free(saved_trimed_name);
		head = head->next;
	}
}
