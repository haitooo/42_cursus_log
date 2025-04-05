/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 20:09:16 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/30 16:43:29 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_words_for_double(char *str)
{
	int	index;

	index = 0;
	if (!ft_strncmp(str, "$?", 2) || !ft_strncmp(str, "$$", 2))
		index = 2;
	else if (str[0] == '$')
	{
		index++;
		if (!str[index] || (!ft_isalpha(str[index]) && str[index] != '_'))
			return (index);
		while (str[index] && (ft_isalnum(str[index]) || str[index] == '_'))
			index++;
	}
	else
	{
		while (str[index] && str[index] != '$')
			index++;
	}
	return (index);
}

t_splited	*get_new_splited_for_double(char *str, int len)
{
	t_splited	*new;

	new = (t_splited *)malloc(sizeof(t_splited));
	if (!new)
		return (NULL);
	new->str = ft_strndup(str, len);
	if (!new->str)
		return (free(new), NULL);
	if (new->str[0] != '$')
		new->type = PLAIN;
	else if (!ft_strcmp(new->str, "$") || !ft_strcmp(new->str, "$$"))
		new->type = PLAIN;
	else
		new->type = QUOTEVAR;
	new->next = NULL;
	return (new);
}

int	count_words_for_plain(char *str)
{
	int	index;

	index = 0;
	if (!ft_strncmp(str, "$?", 2) || !ft_strncmp(str, "$$", 2))
		index = 2;
	else if (str[0] == '$')
	{
		index++;
		if (!str[index] || (!ft_isalpha(str[index]) && str[index] != '_'))
			return (index);
		while (str[index] && (ft_isalnum(str[index]) || str[index] == '_'))
			index++;
	}
	else if (str[index] == '*')
	{
		while (str[index] == '*')
			index++;
	}
	else
	{
		while (str[index] && str[index] != '*' && str[index] != '$')
			index++;
	}
	return (index);
}

t_splited	*get_new_splited_for_plain(char *str, int len)
{
	t_splited	*new;

	new = (t_splited *)malloc(sizeof(t_splited));
	if (!new)
		return (NULL);
	new->str = ft_strndup(str, len);
	if (!new->str)
		return (free(new), NULL);
	if (new->str[0] != '*' && new->str[0] != '$')
		new->type = PLAIN;
	else if (new->str[0] == '*')
		new->type = WILDS;
	else if (!ft_strcmp(new->str, "$") || !ft_strcmp(new->str, "$$"))
		new->type = PLAIN;
	else
		new->type = PLAINVAR;
	new->next = NULL;
	return (new);
}
