/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 11:51:32 by tssaito           #+#    #+#             */
/*   Updated: 2025/07/03 11:51:34 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_init(t_line *ans)
{
	ans->line = NULL;
	ans->len = 1;
	ans->m_size = 0;
}

static int	ft_getchar(int fd, t_buf *buf)
{
	char	c;

	if (buf->len < 1)
	{
		buf->len = read(fd, buf->buf, BUFFER_SIZE);
		if (buf->len == -1)
			return (READ_ERROR);
		if (buf->len == 0)
			return (EOF);
		buf->save = buf->buf;
	}
	buf->len--;
	c = *buf->save;
	buf->save++;
	return (c);
}

static void	add_char(char *ans, char c, size_t len)
{
	ans[len - 1] = c;
	ans[len] = '\0';
}

static int	ft_make_ans(char c, t_line *ans)
{
	char	*tmp;

	if (!ans->m_size)
	{
		ans->m_size = BUFFER_SIZE + 1;
		ans->line = (char *)malloc(sizeof(char) * ans->m_size);
		if (!ans->line)
			return (MALLOC_ERROR);
	}
	else if (ans->len + 1 > ans->m_size)
	{
		if (ans->m_size > SIZE_MAX / 2)
			ans->m_size = SIZE_MAX;
		else
			ans->m_size *= 2;
		tmp = (char *)malloc(sizeof(char) * ans->m_size);
		if (!tmp)
			return (free(ans->line), MALLOC_ERROR);
		ft_memcpy(tmp, ans->line, ans->len);
		free(ans->line);
		ans->line = tmp;
	}
	add_char(ans->line, c, ans->len);
	ans->len++;
	return (0);
}

char	*get_next_line(int fd)
{
	char			c;
	int				check;
	static t_buf	buf;
	t_line			ans;

	if (fd < 0)
		return (NULL);
	ft_init(&ans);
	while (1)
	{
		c = ft_getchar(fd, &buf);
		if (c == READ_ERROR)
			return (free(ans.line), NULL);
		if (c == EOF)
			break ;
		if (ans.len == SIZE_MAX)
			return (free(ans.line), NULL);
		check = ft_make_ans(c, &ans);
		if (check == MALLOC_ERROR)
			return (NULL);
		if (c == '\n')
			break ;
	}
	return (ans.line);
}
