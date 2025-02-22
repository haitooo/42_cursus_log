/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 17:59:26 by haito             #+#    #+#             */
/*   Updated: 2025/02/18 18:51:57 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_pipefd(int **pipefd, int i)
{
	if (!pipefd)
		return ;
	while (--i >= 0)
	{
		close(pipefd[i][0]);
		close(pipefd[i][1]);
		free(pipefd[i]);
	}
	free(pipefd);
}

void	after_call_child(t_cmd arg, int **pipefd)
{
	int	n;

	n = 0;
	free_pipefd(pipefd, arg.size_cmd - 1);
	while (arg.path != NULL && n < arg.size_cmd)
		free(arg.path[n++]);
	free(arg.path);
	if (ft_strcmp(arg.infile_name, "here_doc") == 0)
		unlink("here_doc");
}
