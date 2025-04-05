/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_wildcard.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 20:33:11 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/30 13:09:33 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_len(char *str)
{
	int		index;
	int		j;
	char	ope;

	index = 0;
	while (str[index])
	{
		if (str[index] == '*' || str[index] == '/')
			break ;
		if (str[index] == '\"' || str[index] == '\'')
		{
			ope = str[index];
			j = 1;
			while (str[index + j] && str[index + j] != ope)
				j++;
			if (str[index + j] == ope)
				index += j + 1;
			else
				index++;
		}
		else
			index++;
	}
	return (index);
}

static int	get_dir_len(char *str)
{
	int	index;

	if (str[0] == '/')
		return (1);
	index = 0;
	while (str[index])
	{
		if (ft_strncmp(&str[index], "./", 2))
			break ;
		index += 2;
	}
	return (index);
}

static char	*remove_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != '\'' && str[i] != '\"')
			i++;
		else
			ft_strlcpy(&str[i], &str[i + 1], ft_strlen(&str[i]));
	}
	return (str);
}

static t_words	*make_new_words(char *str, int len)
{
	t_words	*new;

	new = (t_words *)malloc(sizeof(t_words));
	if (!new)
		return (NULL);
	new->name = ft_strndup(str, len);
	if (!new->name)
		return (free(new), NULL);
	if (len == 1 && !ft_strcmp(new->name, "*"))
		new->type = WILD;
	else if (!ft_strcmp(new->name, "/") || !ft_strncmp(new->name, "./", 2))
		new->type = SLASH;
	else
	{
		new->type = ELSE;
		new->name = remove_quotes(new->name);
	}
	new->next = NULL;
	return (new);
}

t_words	*split_wildcards(char *str)
{
	t_words	*words;
	t_words	*prev;
	t_words	*new;
	int		len;

	words = NULL;
	while (*str)
	{
		if (*str == '/' || !ft_strncmp(str, "./", 2))
			len = get_dir_len(str);
		else if (*str == '*')
			len = 1;
		else
			len = get_len(str);
		new = make_new_words(str, len);
		if (!new)
			return (words);
		if (!words)
			words = new;
		else
			prev->next = new;
		str += len;
		prev = new;
	}
	return (words);
}
