/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_words_utils1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 15:42:08 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/24 01:12:34 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*dup_singlequot_text(char *str, char **words, int *i)
{
	char	*end;

	end = str + 1;
	while (*end && *end != '\'')
		end++;
	end++;
	words[*i] = ft_strndup(str, end - str);
	if (!words[*i])
		return (NULL);
	*i += 1;
	if (*end == '\'')
		end++;
	return (end);
}

char	*dup_plaintext_in_doublequote(char *str, char **words, int i)
{
	char	*start;
	char	*end;

	start = str;
	end = str;
	while (*end == '$')
		end++;
	while (*end && *end != '\"' && *end != '$')
		end++;
	words[i] = ft_strndup(start, end - start);
	if (!words[i])
		return (NULL);
	return (end);
}

char	*dup_doublequot_text(char *str, char **words, int *i, t_var **varlist)
{
	char	*start;
	char	*end;

	end = str + 1;
	start = str + 1;
	words[*i] = ft_strdup("\"");
	if (!words[*i])
		return (NULL);
	*i += 1;
	while (*end && *end != '\"')
	{
		if (is_var(start))
			end = dup_var(start, words, HAVE_QUOTE, varlist);
		else
			end = dup_plaintext_in_doublequote(end, words, *i);
		if (!end)
			return (NULL);
		*i += 1;
		start = end;
	}
	words[*i] = ft_strdup("\"");
	if (!words[*i])
		return (NULL);
	*i += 1;
	return (end + 1);
}

char	*dup_plain_text(char *str, char **words, int *i)
{
	char	*end;

	end = str;
	while (*end == '$')
		end++;
	while (*end && *end != '$' && *end != '\'' && *end != '\"')
		end++;
	words[*i] = ft_strndup(str, end - str);
	if (!words[*i])
		return (NULL);
	*i += 1;
	words[*i] = NULL;
	return (end);
}
