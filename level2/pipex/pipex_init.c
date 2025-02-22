/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 17:59:36 by haito             #+#    #+#             */
/*   Updated: 2025/02/18 18:26:36 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	**init_pipefd(t_cmd arg)
{
	int	**pipefd;
	int	i;

	i = -1;
	pipefd = malloc((arg.size_cmd - 1) * sizeof(int *));
	if (!pipefd)
		error_init_pipefd(arg, i, pipefd, 1);
	while (++i < arg.size_cmd - 1)
	{
		pipefd[i] = malloc(2 * sizeof(int));
		if (!pipefd[i])
			error_init_pipefd(arg, i, pipefd, 2);
		if (pipe(pipefd[i]) == -1)
			error_init_pipefd(arg, i, pipefd, 3);
	}
	return (pipefd);
}

pid_t	*init_pids(t_cmd arg, int **pipefd)
{
	pid_t	*pids;
	int		n;

	pids = malloc(arg.size_cmd * sizeof(pid_t));
	n = 0;
	if (!pids)
	{
		perror("malloc error");
		free_pipefd(pipefd, arg.size_cmd - 1);
		if (ft_strcmp(arg.infile_name, "here_doc") == 0)
			unlink("here_doc");
		while (n < arg.size_cmd)
			free(arg.path[n++]);
		free(arg.path);
		exit(1);
	}
	return (pids);
}
