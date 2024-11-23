/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 16:09:47 by haito             #+#    #+#             */
/*   Updated: 2024/11/18 06:36:40 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(const char *s)
{
	int	count;

	count = 0;
	if (s)
	{
		while (s && s[count])
			count++;
	}
	return (count);
}

void	my_free(char **str)
{
	if (str && *str)
	{
		free(*str);
		*str = NULL;
	}
}

char	*add_chr(char *line, char c)
{
	char	*new_line;
	char	*starting_point;
	size_t	sizeof_line;

	sizeof_line = ft_strlen(line) + 1;
	new_line = (char *)malloc(sizeof_line + 1);
	if (!new_line)
		return (NULL);
	starting_point = new_line;
	if (line)
	{
		while (*line)
			*new_line++ = *line++;
	}
	*new_line++ = c;
	*new_line = '\0';
	return (starting_point);
}
