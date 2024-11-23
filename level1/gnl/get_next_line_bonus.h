/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 04:45:50 by haito             #+#    #+#             */
/*   Updated: 2024/11/18 07:04:21 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 256

# endif

typedef struct s_lst
{
	int				fd_num;
	char			buf[BUFFER_SIZE + 1];
	int				buf_index;
	int				read_bytes;
	struct s_lst	*next;
}t_lst;

char	*get_next_line(int fd);
size_t	ft_strlen(const char *s);
char	*add_chr(char *line, char c);
void	my_free(char **str);
t_lst	*get_node(t_lst **fd_lst, int fd);

#endif
