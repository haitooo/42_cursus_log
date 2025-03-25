/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_split2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 21:59:00 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/22 17:09:13 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_size(char *str)
{
	int	size;
	int	i;

	size = 0;
	i = 0;
	while (str[i])
	{
		i += get_words_len(&str[i]);
		size++;
	}
	return (size);
}

char	**split_words2(char *str)
{
	int		i;
	int		len;
	int		size;
	char	**words;

	size = get_size(str);
	words = (char **)malloc(sizeof(char *) * (size + 1));
	if (!words)
		return (NULL);
	i = 0;
	while (*str)
	{
		while (*str == ' ')
			str++;
		len = get_words_len(str);
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
