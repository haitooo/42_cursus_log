/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 17:59:18 by haito             #+#    #+#             */
/*   Updated: 2025/02/18 19:47:00 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error_fork(t_cmd arg, int **pipefd, pid_t *pids)
{
	int	n;

	n = 0;
	perror("fork failed");
	free_pipefd(pipefd, arg.size_cmd - 1);
	if (pids)
		free(pids);
	if (ft_strcmp(arg.infile_name, "here_doc") == 0)
		unlink("here_doc");
	while (n <= arg.size_cmd)
		free(arg.path[n++]);
	free(arg.path);
	exit(1);
}

char	**error_in_get_path(char **path, int errornum, int n)
{
	if (errornum == 1)
	{
		ft_fprintf("malloc failed: %s\n", strerror(errno));
		while (--n > 0)
			free(path[n]);
	}
	free(path);
	return (NULL);
}

void	error_init_pipefd(t_cmd arg, int i, int **pipefd, int errornum)
{
	int	n;

	n = 0;
	if (errornum == 1 || errornum == 2)
		ft_fprintf("malloc failed: %s\n", strerror(errno));
	if (errornum == 3)
		perror("pipe error");
	if (errornum == 2 || errornum == 3)
		free_pipefd(pipefd, i);
	if (ft_strcmp(arg.infile_name, "here_doc") == 0)
		unlink("here_doc");
	while (n < arg.size_cmd)
		free(arg.path[n++]);
	free(arg.path);
	exit(1);
}

void	error_here_doc(int fd, const char *lmt, int errornum)
{
	if (errornum == 1)
		ft_fprintf("bash: here_doc: %s\n", strerror(errno));
	if (errornum == 2)
	{
		ft_fprintf("bash: here_doc: %s\n", strerror(errno));
		close(fd);
	}
	if (errornum == 3)
	{
		ft_fprintf("\nbash: warning: here_document wanted %s\n", lmt);
		unlink("here_doc");
		close(fd);
	}
	exit (1);
}
