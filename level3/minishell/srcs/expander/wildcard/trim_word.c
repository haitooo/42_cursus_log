/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trim_word.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 18:23:38 by tssaito           #+#    #+#             */
/*   Updated: 2025/04/03 16:47:47 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*trim_word_start(t_wild **file, t_splited **splited, char *name)
{
	char		*trimed_name;
	t_wild		*target;
	t_splited	*head;

	target = *file;
	if (!target->flag || !name)
		return (NULL);
	trimed_name = ft_strdup(name);
	if (!trimed_name)
		return (NULL);
	head = *splited;
	if (head && head->type == WILDS)
		return (trimed_name);
	if (head && ft_strncmp(trimed_name, head->str, ft_strlen(head->str)))
	{
		target->flag = 0;
		return (free(trimed_name), NULL);
	}
	if (head)
		ft_strlcpy(trimed_name, &trimed_name[ft_strlen(head->str)],
			ft_strlen(&trimed_name[ft_strlen(head->str)]) + 1);
	return (trimed_name);
}

static char	*set_flag(t_wild **file)
{
	t_wild	*target;

	target = *file;
	target->flag = 0;
	return (NULL);
}

char	*trim_word_end(t_wild **file, t_splited **splited, char *name)
{
	t_wild		*target;
	t_splited	*head;
	int			wlen;
	int			nlen;

	target = *file;
	if (!target->flag || !name || !*name)
		return (NULL);
	head = *splited;
	while (head && head->next && head->next->str[0] != '/')
		head = head->next;
	if (head && head->type == WILDS)
		return (name);
	nlen = ft_strlen(name) - 1;
	wlen = -1;
	if (head && head->str)
		wlen = ft_strlen(head->str) - 1;
	if (nlen < 0 || wlen < 0)
		return (set_flag(file));
	while (head && wlen >= 0 && nlen >= 0)
		if (head->str[wlen--] != name[nlen--])
			return (set_flag(file));
	name[nlen + 1] = '\0';
	return (name);
}
