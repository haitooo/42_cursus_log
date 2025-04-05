/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/30 19:00:07 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/31 10:03:01 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*make_filename(char *basename, int i)
{
	char	*index;
	char	*file;

	index = ft_itoa(i);
	if (!index)
		return (perror("minishell: malloc failed"), NULL);
	file = ft_strjoin(basename, index);
	free(index);
	if (!file)
		return (perror("minishell: malloc failed"), NULL);
	return (file);
}

static char	*get_filename(void)
{
	char	*file;
	char	*basename;
	int		i;

	basename = ft_strdup("/tmp/.tmp");
	if (!basename)
		return (perror("minishell: malloc failed"), NULL);
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
		return (ERROR);
	fd = open(*file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (free(*file), perror("minishell: open failed"), -1);
	return (fd);
}

void	child_heredoc(t_heredoc *h, char *limiter, t_var **varlist,
		t_status **st_head)
{
	if (signal(SIGINT, sig_handler_heredoc) == SIG_ERR)
	{
		perror("minishell: signal");
		exit(1);
	}
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
	{
		perror("minishell: signal");
		exit(1);
	}
	heredoc_loop(h->filefd, limiter, varlist);
	free_lst_status(*st_head, NULL);
	free_varlist(varlist);
	free(h->file);
	if (g_signal == SIGINT)
		exit(1);
	exit(0);
}

char	*here_doc(char *limiter, t_var **varlist, t_status **st_head)
{
	t_heredoc	h;
	pid_t		pid;
	int			status;

	status = 0;
	h.file = NULL;
	h.filefd = open_tmpfile(&h.file);
	if (h.filefd == -1)
		return (NULL);
	pid = fork();
	if (pid == 0)
		child_heredoc(&h, limiter, varlist, st_head);
	waitpid(pid, &status, 0);
	g_signal = 0;
	if (close(h.filefd) == -1)
		return (perror("minishell: close failed"), free(h.file), NULL);
	if (status == 256)
	{
		update_exit_code(130, varlist);
		if (h.file)
			unlink(h.file);
		free(h.file);
		return (NULL);
	}
	return (h.file);
}
