/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 04:45:47 by haito             #+#    #+#             */
/*   Updated: 2024/11/18 07:17:21 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

int	read_text(t_lst *s, char **line, int fd)
{
	if (s->buf_index >= s->read_bytes)
	{
		s->read_bytes = read(fd, s->buf, BUFFER_SIZE);
		if (s->read_bytes <= 0)
		{
			if (*line && **line)
				return (0);
			else
				return (1);
		}
		s->buf[s->read_bytes] = '\0';
		s->buf_index = 0;
	}
	return (-1);
}

char	*make_str(t_lst *s, char **line)
{
	char	*tmp;

	tmp = add_chr(*line, s->buf[s->buf_index]);
	if (!tmp)
		return (NULL);
	free(*line);
	*line = tmp;
	s->buf_index++;
	return (*line);
}

void	add_endl(char **line)
{
	char	*tmp;

	tmp = add_chr(*line, '\n');
	free(*line);
	*line = tmp;
}

char	*get_next_line(int fd)
{
	static t_lst	*fd_lst;
	t_lst			*s;
	char			*line;
	int				result;

	s = get_node(&fd_lst, fd);
	if (!s)
		return (NULL);
	line = NULL;
	while (1)
	{
		result = read_text(s, &line, fd);
		if (result == 1)
			return (my_free(&line), NULL);
		if (result == 0)
			return (line);
		while (s->buf_index < s->read_bytes && s->buf[s->buf_index] != '\n')
			if (!make_str(s, &line))
				return (my_free(&line), NULL);
		if (s->buf_index < s->read_bytes && s->buf[s->buf_index++] == '\n')
			break ;
	}
	add_endl(&line);
	return (line);
}

// #include <stdio.h>

// int	main(void)
// {
// 	int		fd;
// 	char	*current_line;

// 	fd = open("test.txt", O_RDONLY);
// 	if (fd == -1)
// 	{
// 		perror("Error opening file");
// 		return (1);
// 	}
// 	current_line = NULL;
// 	while (1)
// 	{
// 		current_line = get_next_line(fd);
// 		if (current_line == NULL)
// 			break ;
// 		printf("%s", current_line);
// 		free(current_line);
// 	}
// 	// current_line = get_next_line(fd);
// 	// printf("%s", current_line);
// 	// free(current_line);
// 	close(fd);
// 	close(fd2);
// 	return (0);
// }
