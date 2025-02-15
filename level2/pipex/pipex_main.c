/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:31:04 by haito             #+#    #+#             */
/*   Updated: 2025/02/13 19:32:33 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	execute_command(t_cmd arg, char **envp, int i)
{
	char	**cmd;
	char	*path;

	if (arg.path[i] == NULL && access(arg.cmds[i], X_OK) == 0)
	{
		cmd = cmd_from_path(arg, i);
		path = arg.cmds[i];
		execve(path, cmd, envp);
		execve_from_path(arg, cmd, 4);
	}
	if (arg.path[i] == NULL || ft_strlen(arg.cmds[i]) == 0)
	{
		ft_fprintf("%s: command not found\n", arg.cmds[i]);
		path_free(arg);
		exit(127);
	}
	cmd = ft_split(arg.cmds[i], ' ');
	if (!cmd)
		exit(1);
	path = arg.path[i];
	execve(path, cmd, envp);
	perror("exec failed");
	execve_free(arg, cmd, i);
	exit(1);
}

int	make_pipe(t_cmd arg, char **envp)
{
	int		**pipefd;
	pid_t	*pids;
	int		i;
	int		ret;

	i = -1;
	pipefd = init_pipefd(arg);
	pids = init_pids(arg, pipefd);
	while (++i < arg.size_cmd)
	{
		pids[i] = fork();
		if (pids[i] == -1)
			error_fork(arg, pipefd, pids);
		if (pids[i] == 0)
		{
			free(pids);
			continue_child(pipefd, arg, i, envp);
		}
	}
	after_call_child(arg, pipefd);
	ret = wait_child(arg, pids);
	free(pids);
	return (ret);
}

void	get_from_stdin(const char *lmt)
{
	int		fd;
	char	*line;

	fd = open("here_doc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		error_here_doc(-1, 1);
	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(0);
		if (!line)
			break ;
		if (!ft_strncmp(line, lmt, ft_strlen(lmt))
			&& line[ft_strlen(lmt)] == '\n')
		{
			free(line);
			break ;
		}
		if (write(fd, line, ft_strlen(line)) == -1)
			error_here_doc(fd, 2);
		free(line);
	}
	close(fd);
}

void	check_here_doc(t_cmd *arg, char **argv, int argc)
{
	if (ft_strcmp(argv[1], "here_doc") == 0)
	{
		get_from_stdin(argv[2]);
		arg->cmds = argv + 3;
		arg->size_cmd = argc - 4;
	}
	else
	{
		arg->cmds = argv + 2;
		arg->size_cmd = argc - 3;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_cmd	arg;
	int		has_access_error;
	int		ret;

	has_access_error = 0;
	if (argc < 5)
		return (write(2, "Error\n", 6), 1);
	if (access(argv[argc - 1], F_OK) == 0
		&& access(argv[argc - 1], W_OK) == -1)
		has_access_error = 1;
	check_here_doc(&arg, argv, argc);
	arg.infile_name = argv[1];
	arg.outfile_name = argv[argc - 1];
	arg.path = get_path(arg, envp);
	if (!arg.path)
		error_get_path(arg);
	ret = make_pipe(arg, envp);
	if (has_access_error)
		return (1);
	return (ret);
}
