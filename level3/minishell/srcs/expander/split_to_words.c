/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_to_words.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 13:43:31 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/31 10:54:47 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_splited	*split_for_plain(char *str)
{
	t_splited	*splited;
	t_splited	*prev;
	t_splited	*new;
	int			i;
	int			len;

	i = 0;
	splited = NULL;
	while (str[i])
	{
		len = count_words_for_plain(&str[i]);
		new = get_new_splited_for_plain(&str[i], len);
		if (!new)
			return (free_splited(&splited), NULL);
		if (!splited)
			splited = new;
		else
			prev->next = new;
		prev = new;
		i += len;
	}
	return (splited);
}

static t_splited	*split_for_double(char *str)
{
	t_splited	*splited;
	t_splited	*prev;
	t_splited	*new;
	int			i;
	int			len;

	i = 0;
	splited = NULL;
	while (str[i])
	{
		len = count_words_for_double(&str[i]);
		new = get_new_splited_for_double(&str[i], len);
		if (!new)
			return (free_splited(&splited), NULL);
		if (!splited)
			splited = new;
		else
			prev->next = new;
		prev = new;
		i += len;
	}
	return (splited);
}

static t_splited	*split_for_null(void)
{
	t_splited	*new;

	new = (t_splited *)malloc(sizeof(t_splited));
	if (!new)
		return (NULL);
	new->type = PLAIN;
	new->str = ft_strdup("");
	if (!new->str)
		return (free(new), NULL);
	new->next = NULL;
	return (new);
}

static t_splited	*split_for_single(char *str)
{
	t_splited	*new;

	new = (t_splited *)malloc(sizeof(t_splited));
	if (!new)
		return (NULL);
	new->type = PLAIN;
	new->str = ft_strdup(str);
	if (!new->str)
		return (free(new), NULL);
	new->next = NULL;
	return (new);
}

t_splited	*split_to_words(t_splited *pre_splited)
{
	t_splited	*new;

	new = NULL;
	if (!pre_splited->str[0])
		new = split_for_null();
	else if (pre_splited->type == PLAIN)
		new = split_for_plain(pre_splited->str);
	else if (pre_splited->type == DOUBLE)
		new = split_for_double(pre_splited->str);
	else if (pre_splited->type == SINGLE)
		new = split_for_single(pre_splited->str);
	return (new);
}
