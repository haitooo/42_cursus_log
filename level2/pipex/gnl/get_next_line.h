/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 16:09:42 by haito             #+#    #+#             */
/*   Updated: 2025/02/18 19:43:54 by haito            ###   ########.fr       */
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
}	t_buffers;

char	*get_next_line(int fd, const char *lmt);
size_t	ft_strlen(const char *str);
char	*add_chr(char *line, char c);
void	gnl_my_free(char **str);
void	error_here_doc(int fd, const char *lmt, int errornum);

#endif
