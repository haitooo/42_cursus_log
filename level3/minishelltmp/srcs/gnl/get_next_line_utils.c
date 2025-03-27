/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 17:58:40 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/26 15:04:51 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	ft_init(t_line *ans)
{
	ans->line = NULL;
	ans->len = 1;
	ans->m_size = 0;
}

int	ft_getchar(int fd, t_buf *buf)
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

int	ft_make_ans(char c, t_line *ans)
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
	add_chars(ans->line, c, ans->len);
	ans->len++;
	return (0);
}

void	add_chars(char *ans, char c, size_t len)
{
	ans[len - 1] = c;
	ans[len] = '\0';
}
