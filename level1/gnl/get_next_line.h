/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 16:09:42 by haito             #+#    #+#             */
/*   Updated: 2024/11/18 07:04:15 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 256

# endif

typedef struct s_buffers
{
	char	buf[BUFFER_SIZE + 1];
	int		buf_index;
	int		read_bytes;
}t_buffers;

char	*get_next_line(int fd);
size_t	ft_strlen(const char *s);
char	*add_chr(char *line, char c);
void	my_free(char **str);

#endif
