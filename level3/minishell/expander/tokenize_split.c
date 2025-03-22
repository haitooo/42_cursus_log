/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_split.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 21:59:00 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/22 03:29:19 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_len_quote(char *str)
{
	int		i;
	char	ope;

	i = 0;
	while (str[i] == ' ')
		i++;
	while (str[i])
	{
		while (str[i] && str[i] != '\'' && str[i] != '\"' && str[i] != ' ')
			i++;
		if (str[i] == ' ' || !str[i])
			break ;
		if (str[i] == '\'' || str[i] == '\"')
		{
			ope = str[i];
			i++;
			while (str[i] && str[i] != ope)
				i++;
			if (str[i] == ope)
				i++;
		}
	}
	return (i);
}

static int	get_words_len(char *str, t_type flag)
{
	int	i;

	if (flag)
		return (get_len_quote(str));
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
			return (ft_strlen(str));
		i++;
	}
	i = 0;
	while (str[i] && str[i] == ' ')
		i++;
	while (str[i] && str[i] != ' ')
		i++;
	return (i);
}

static int	get_size(char *str, t_type flag)
{
	int	size;
	int	i;

	size = 0;
	i = 0;
	while (str[i])
	{
		i += get_words_len(&str[i], flag);
		size++;
	}
	return (size);
}

char	**split_words(char *str, t_type flag)
{
	int		i;
	int		len;
	int		size;
	char	**words;

	size = get_size(str, flag);
	words = (char **)malloc(sizeof(char *) * (size + 1));
	if (!words)
		return (NULL);
	i = 0;
	while (*str)
	{
		while (*str == ' ')
			str++;
		len = get_words_len(str, flag);
		words[i] = (char *)malloc(sizeof(char) * (len + 1));
		if (!words[i])
			return (NULL);
		ft_strlcpy(words[i], str, len + 1);
		i++;
		str += len;
	}
	words[i] = NULL;
	return (words);
}
