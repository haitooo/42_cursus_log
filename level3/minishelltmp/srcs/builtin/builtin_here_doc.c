/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_here_doc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 23:09:01 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/26 15:48:11 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*make_filename(char *basename, int i)
{
	char	*index;
	char	*file;

	index = ft_itoa(i);
	if (!index)
	{
		builtin_error(errno, "cannot create temp file for here-document");
		return (NULL);
	}
	file = ft_strjoin(basename, index);
	free(index);
	if (!file)
	{
		builtin_error(errno, "cannot create temp file for here-document");
		return (NULL);
	}
	return (file);
}

static char	*get_filename(void)
{
	char	*file;
	char	*basename;
	int		i;

	basename = ft_strdup("/tmp/.tmp");
	if (!basename)
		builtin_error(errno, "cannot create temp file for here-document");
	if (!basename)
		return (NULL);
	file = basename;
	while (!access(file, F_OK))
	{
		i = 0;
		while (i <= INT_MAX)
		{
			file = make_filename(basename, i);
			if (access(file, F_OK))
				break ;
			free(file);
			i++;
		}
		free(basename);
		basename = file;
	}
	return (file);
}

static int	open_tmpfile(char **file)
{
	int	fd;

	if (!*file)
		*file = get_filename();
	if (!*file)
		return (EXIT_FAILURE);
	fd = open(*file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (builtin_error(errno, *file));
	return (fd);
}

static int	builtin_heredoc(int fd, char *limiter, t_type type, t_var **varlist)
{
	int	save_stdin;

	save_stdin = STDIN_FILENO;
	if (signal(SIGINT, sig_handler_heredoc) == SIG_ERR)
		return (perror("minishell: signal"), FAILED);
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		return (perror("minishell: signal"), FAILED);
	heredoc_loop_builtin(fd, limiter, type, varlist);
	if (g_signal == SIGINT)
		g_signal = 0;
	if (signal(SIGINT, sig_handler_inprocess) == SIG_ERR)
		return (perror("minishell: signal"), FAILED);
	if (signal(SIGQUIT, sig_handler_inprocess) == SIG_ERR)
		return (perror("minishell: signal"), FAILED);
	dup2(STDIN_FILENO, save_stdin);
	return (SUCCESS);
}

int	check_here_doc(t_tokens **tokens, char **tmpfile, t_var **varlist)
{
	t_tokens	*head;
	int			status;
	int			fd;

	head = *tokens;
	while (head)
	{
		if (head->type == HEREDOC)
		{
			fd = open_tmpfile(tmpfile);
			if (fd < 0)
				return (EXIT_FAILURE);
			status = builtin_heredoc(fd, head->next->token, head->type,
					varlist);
			if (close(fd) == -1)
				return (builtin_error(errno, *tmpfile));
			if (status != SUCCESS)
				return (unlink(*tmpfile), free(*tmpfile), EXIT_FAILURE);
		}
		head = head->next;
	}
	return (EXIT_SUCCESS);
}
