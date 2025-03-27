/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_words_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:42:08 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/24 01:24:18 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*skip_top_space(char *str, int forward)
{
	int	i;

	i = 0;
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (str[i] && !forward)
		ft_strlcpy(str, &str[i], ft_strlen(&str[i]) + 1);
	else if (i && str[i])
		ft_strlcpy(str, &str[i - 1], ft_strlen(&str[i - 1]) + 1);
	return (str);
}

char	*skip_space(char *str, char next)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		while (str[i] && !ft_isspace(str[i]))
			i++;
		j = 0;
		while (str[i + j] && ft_isspace(str[i + j]))
			j++;
		if (j && str[i + j])
			ft_strlcpy(&str[i + 1], &str[i + j], ft_strlen(&str[i + j]) + 1);
		else if (j && next)
			ft_strlcpy(&str[i], " ", 2);
		else
			str[i] = '\0';
		while (str[i] && ft_isspace(str[i]))
			i++;
	}
	return (str);
}

static int	get_namelen(char *str)
{
	int	index;

	index = 0;
	if (str[index] == '?')
		index++;
	else
	{
		while (str[index] && (ft_isalnum(str[index]) || str[index] == '_'))
			index++;
	}
	return (index);
}

static char	*set_var(char *value, char end, t_type type, int index)
{
	char	*new_word;

	new_word = ft_strdup(value);
	if (!new_word)
		return (NULL);
	if (type != HAVE_QUOTE)
	{
		new_word = skip_top_space(new_word, index);
		new_word = skip_space(new_word, end);
	}
	return (new_word);
}

char	*dup_var(char *str, char **words, t_type type, t_var **varlist)
{
	char	*end;
	char	*name;
	t_var	*var;
	int		i;

	end = str + get_namelen(str + 1) + 1;
	name = ft_strndup(str + 1, end - str - 1);
	if (!name)
		return (NULL);
	var = get_var(varlist, name);
	free(name);
	i = 0;
	while (words[i])
		i++;
	if (!var || !var->value)
		words[i] = ft_strdup("");
	else
		words[i] = set_var(var->value, *end, type, i);
	if (!words[i])
		return (NULL);
	return (end);
}
