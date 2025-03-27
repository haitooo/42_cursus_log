/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 17:58:35 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/26 16:06:15 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>
# include <stdint.h>
# include <stdio.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# if BUFFER_SIZE < 0 || BUFFER_SIZE > 99999999
#  error BUFFER_SIZE must be greater than 0 and less than 100000000
# endif

# define READ_ERROR -22
# define MALLOC_ERROR -42

typedef struct s_buf
{
	char	buf[BUFFER_SIZE];
	char	*save;
	int		len;
}			t_buf;

typedef struct s_line
{
	char	*line;
	size_t	len;
	size_t	m_size;
}			t_line;

char		*get_next_line_for_heredoc(int fd);
void		add_chars(char *ans, char c, size_t len);
void		*ft_memcpy(void *dest, const void *src, size_t n);
int			ft_getchar(int fd, t_buf *buf);
int			ft_make_ans(char c, t_line *ans);
void		ft_init(t_line *ans);

#endif
