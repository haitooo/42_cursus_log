/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trim_space.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 02:54:52 by haito             #+#    #+#             */
/*   Updated: 2025/03/26 02:39:46 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*trim_edges_space(const char *str)
{
	size_t	len;

	while (*str && *str == ' ')
		str++;
	len = ft_strlen(str);
	while (len > 0 && str[len - 1] == ' ')
		len--;
	return (ft_strndup(str, len));
}

void	update_quote_state(char c, int *in_single_quote, int *in_double_quote)
{
	if (c == '\'' && !(*in_double_quote))
		*in_single_quote = !(*in_single_quote);
	if (c == '"' && !(*in_single_quote))
		*in_double_quote = !(*in_double_quote);
}

char	*cpy_trimed(char *result, char *trimmed)
{
	int		i;
	int		j;
	int		in_single_quote;
	int		in_double_quote;
	int		len;

	i = -1;
	j = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	len = ft_strlen(trimmed);
	while (++i < len)
	{
		update_quote_state(trimmed[i], &in_single_quote, &in_double_quote);
		if (trimmed[i] == ' ' && !in_single_quote && !in_double_quote
			&& j > 0 && result[j - 1] == ' ')
			continue ;
		result[j++] = trimmed[i];
	}
	result[j] = '\0';
	return (result);
}

char	*trim_spaces(char *str)
{
	char	*trimmed;
	char	*result;
	int		len;

	trimmed = trim_edges_space(str);
	if (!trimmed)
		return (NULL);
	free(str);
	len = ft_strlen(trimmed);
	result = (char *)malloc(len + 1);
	if (!result)
		return (free(trimmed), NULL);
	result = cpy_trimed(result, trimmed);
	return (free(trimmed), result);
}
