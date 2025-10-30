/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 22:29:31 by haito             #+#    #+#             */
/*   Updated: 2025/10/30 22:55:27 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "get_next_line.h"



char	*get_next_line(int fd)
{
	char		*line;
	char		*tmp;
	static char	*stash;
	char		buffer[BUFFER_SIZE + 1];
	ssize_t		bytes_read;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	while (1)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			write(2, "Error\n", 6);
			cleanup();
			return (NULL);
		}
		else if (bytes_read == 0)
		{
			cleanup();
			return (NULL);
		}
		else
		{
			buffer[bytes_read] = '\0';
			tmp = ft_strjoin(line, buffer);
			free(line);
			line = tmp;
			if (has_breakline(line))
				break ;
		}
	}
	tmp = my_cutstr(line, buffer);
	free(line);
	line = tmp;
	return (line);
}

int	main(void)
{

}
