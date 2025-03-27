/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_here_doc_help.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 05:48:23 by haito             #+#    #+#             */
/*   Updated: 2025/03/26 22:47:09 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc_loop_builtin(int fd, char *limiter, t_type type,
	t_var **varlist)
{
	int		lim_len;
	char	*buf;

	lim_len = ft_strlen(limiter);
	while (1)
	{
		buf = NULL;
		write(STDERR_FILENO, "> ", 2);
		buf = readline(NULL);
		if (g_signal == SIGINT || (buf && !ft_strncmp(limiter, buf, lim_len)))
		{
			free(buf);
			break ;
		}
		else if (!buf)
		{
			ft_eprintf("minishell: warning: here-document wanted `%s'",
				limiter);
			break ;
		}
		if (type == WORD)
			buf = heredoc_expand_var(buf, varlist);
		ft_putendl_fd(buf, fd);
		free(buf);
	}
}
