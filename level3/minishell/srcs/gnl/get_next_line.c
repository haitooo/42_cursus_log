/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 17:58:43 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/26 16:12:32 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line_for_heredoc(int fd)
{
	char			c;
	int				check;
	static t_buf	buf;
	t_line			ans;

	if (fd < 0)
		return (NULL);
	ft_init(&ans);
	write(STDERR_FILENO, "> ", 2);
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
