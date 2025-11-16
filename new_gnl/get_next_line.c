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

#include "get_next_line.h"



char	*get_next_line(int fd)
{
	static char*	stash;
	char*			line;
	char			*buf[BUFFER_SIZE + 1];
	ssize_t			bytes_read;

	if (BUFFER_SIZE <= 0)
		return (NULL);
	while (1)
	{
		if (stash != NULL)
		{
			if (!ft_strchr(stash, '\n'))
			{
				ft_strjoin(line, stash);
				free(stash);
				stash = NULL;
			}
			else
			{
				//改行までをreturnして、残りをstashに入れる。
			}
		}
		else
		{
			bytes_read = read(fd, buf, BUFFER_SIZE);
			if (bytes_read == -1)
			{
				//Error;
			}
			else if (bytes_read == 0)
			{
				//EOF;
			}
			else
			{
				buf[bytes_read] = '\0';
			}
		}
	}
	return (line);
}

int	main(void)
{

}
