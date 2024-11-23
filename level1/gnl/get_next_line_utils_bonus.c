/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 04:45:52 by haito             #+#    #+#             */
/*   Updated: 2024/11/18 06:48:21 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

t_lst	*get_node(t_lst **fd_lst, int fd)
{
	t_lst	*s;
	t_lst	*new_node;

	s = *fd_lst;
	while (s)
	{
		if (s->fd_num == fd)
			return (s);
		s = s->next;
	}
	new_node = (t_lst *)malloc(sizeof(t_lst));
	if (!new_node)
		return (NULL);
	new_node->fd_num = fd;
	new_node->buf[0] = '\0';
	new_node->buf_index = 0;
	new_node->read_bytes = 0;
	new_node->next = *fd_lst;
	*fd_lst = new_node;
	return (new_node);
}

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
