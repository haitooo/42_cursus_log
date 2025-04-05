/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 13:43:31 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/30 21:18:35 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_pre_splited_len(char *str)
{
	char	ope;
	int		i;

	i = 0;
	if (str[i] == '\'' || str[i] == '\"')
	{
		ope = str[i];
		i++;
		while (str[i] != ope)
			i++;
		i++;
	}
	else if (str[i] == '$' && (str[i + 1] == '$' || str[i + 1] == '?'))
		return (2);
	else if (str[i] == '$')
	{
		i++;
		while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
			i++;
	}
	else
		while (str[i] && str[i] != '\'' && str[i] != '\"' && str[i] != '$')
			i++;
	return (i);
}

static t_splited	*get_new_pre_splited(char *str)
{
	t_splited	*new;
	int			len;

	len = get_pre_splited_len(str);
	new = (t_splited *)malloc(sizeof(t_splited));
	if (!new)
		return (NULL);
	new->next = NULL;
	new->str = ft_strndup(str, len);
	if (!new->str)
		return (free(new), NULL);
	if (new->str[0] != '\'' && new->str[0] != '\"')
		new->type = PLAIN;
	else
	{
		new->str[len - 1] = '\0';
		if (new->str[0] == '\'')
			new->type = SINGLE;
		else
			new->type = DOUBLE;
		ft_strlcpy(new->str, &new->str[1], len);
	}
	return (new);
}

t_splited	*pre_split_tokens(char *str)
{
	t_splited	*splited;
	t_splited	*new;
	t_splited	*prev;
	int			i;

	i = 0;
	splited = NULL;
	while (str[i])
	{
		new = get_new_pre_splited(&str[i]);
		if (!new)
			return (free_splited(&splited), NULL);
		if (!splited)
			splited = new;
		else
			prev->next = new;
		prev = new;
		i += get_pre_splited_len(&str[i]);
	}
	return (splited);
}

t_splited	*split_tokens(t_splited *pre_splited)
{
	t_splited	*splited;
	t_splited	*new;
	t_splited	*prev;
	t_splited	*tmp;

	splited = NULL;
	while (pre_splited)
	{
		new = split_to_words(pre_splited);
		if (!new)
			return (free_splited(&splited), free_splited(&pre_splited), NULL);
		if (!splited)
			splited = new;
		else
			prev->next = new;
		while (new && new->next)
			new = new->next;
		prev = new;
		tmp = pre_splited;
		pre_splited = pre_splited->next;
		free(tmp->str);
		free(tmp);
	}
	return (splited);
}
