/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_loop.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 13:41:50 by tssaito           #+#    #+#             */
/*   Updated: 2025/04/03 16:54:11 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc_loop(int filefd, char *limiter, t_var **varlist)
{
	char	*buf;
	int		lim_len;

	lim_len = ft_strlen(limiter);
	g_signal = 0;
	while (1)
	{
		buf = readline("> ");
		if (g_signal == SIGINT || (buf && !lim_len && !*buf)
			|| (buf && lim_len && !ft_strcmp(limiter, buf)))
		{
			free(buf);
			break ;
		}
		else if (!buf)
		{
			ft_eprintf("minishell: warning: here-document wanted `%s'\n",
				limiter);
			break ;
		}
		buf = heredoc_expand_var(buf, varlist);
		ft_putendl_fd(buf, filefd);
		free(buf);
	}
	close(filefd);
}
