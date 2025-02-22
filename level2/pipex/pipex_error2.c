/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_error2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 17:59:23 by haito             #+#    #+#             */
/*   Updated: 2025/02/18 17:59:24 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error_open(t_cmd arg, int **pipefd, int *fd, int errornum)
{
	int	n;

	n = 0;
	if (errornum == 1)
		ft_fprintf("bash: %s: %s\n", arg.infile_name, strerror(errno));
	if (errornum == 6)
		ft_fprintf("bash: %s: %s\n", arg.outfile_name, strerror(errno));
	if (errornum == 2 || errornum == 4)
		perror("dup2 failed (stdin)");
	if (errornum == 3 || errornum == 5)
		perror("dup2 failed (stdout)");
	if (errornum == 2 || errornum == 3)
		close(*fd);
	while (n <= arg.size_cmd)
		free(arg.path[n++]);
	free(arg.path);
	if (ft_strcmp(arg.infile_name, "here_doc") == 0)
		unlink("here_doc");
	free_pipefd(pipefd, arg.size_cmd - 1);
	exit(1);
}

void	execve_free(t_cmd arg, char **cmd, int i)
{
	int	n;

	n = 0;
	while (n < arg.size_cmd)
		free(arg.path[n++]);
	free(arg.path);
	n = count_words(arg.cmds[i], ' ') + 1;
	while (n >= 0)
		free(cmd[n--]);
	free(cmd);
}

void	execve_from_path(t_cmd arg, char **cmd, int errornum)
{
	int	n;

	n = 0;
	while (n < arg.size_cmd)
		free(arg.path[n++]);
	free(arg.path);
	if (errornum == 4)
		free(cmd[1]);
	if (errornum == 3 || errornum == 4)
		free(cmd[0]);
	if (errornum == 2 || errornum == 3 || errornum == 4)
		free(cmd);
	if (errornum == 1 || errornum == 2 || errornum == 3)
	{
		ft_fprintf("malloc failed: %s\n", strerror(errno));
		exit(1);
	}
	perror("exec failed");
	exit(127);
}

void	path_free(t_cmd arg)
{
	int	n;

	n = 0;
	while (n <= arg.size_cmd)
		free(arg.path[n++]);
	free(arg.path);
}
