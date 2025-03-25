/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_vars_split2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:42:08 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/22 17:09:07 by tssaito          ###   ########.fr       */
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

char	*dup_var(char *str, char **words, t_type type, t_var **varlist)
{
	char	*end;
	char	*name;
	t_var	*var;
	int		i;

	end = str;
	end++;
	if (*end == '?')
		end++;
	else
		while (*end && (ft_isalnum(*end) || *end == '_'))
			end++;
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
		words[i] = ft_strdup(var->value);
	if (!words[i])
		return (NULL);
	words[i + 1] = NULL;
	if (type != HAVE_QUOTE)
	{
		words[i] = skip_top_space(words[i], i);
		words[i] = skip_space(words[i], *end);
	}
	return (end);
}
