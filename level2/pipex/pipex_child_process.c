#include "pipex.h"

void	case_first_cmd(int	*fd, t_cmd arg, int **pipefd)
{
	*fd = open(arg.infile_name, O_RDONLY);
	if (*fd < 0)
		error_open(arg, pipefd, fd, 1);
	if (dup2(*fd, STDIN_FILENO) == -1)
		error_open(arg, pipefd, fd, 2);
	if (dup2(pipefd[0][1], STDOUT_FILENO) == -1)
		error_open(arg, pipefd, fd, 3);
	close(*fd);
}

void	case_last_cmd(int *fd, t_cmd arg, int **pipefd)
{
	if (strcmp(arg.infile_name, "here_doc") == 0)
		*fd = open(arg.outfile_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		*fd = open(arg.outfile_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*fd < 0)
		error_open(arg, pipefd, fd, 6);
	if (dup2(pipefd[arg.size_cmd - 2][0], STDIN_FILENO) == -1)
		error_open(arg, pipefd, fd, 2);
	if (dup2(*fd, STDOUT_FILENO) == -1)
		error_open(arg, pipefd, fd, 3);
	close(*fd);
}

void	continue_child(int **pipefd, t_cmd arg, int i, char **envp)
{
	int	fd;

	if (i == 0)
		case_first_cmd(&fd, arg, pipefd);
	else if (i > 0 && i < arg.size_cmd - 1)
	{
		if (dup2(pipefd[i - 1][0], STDIN_FILENO) == -1)
			error_open(arg, pipefd, &fd, 4);
		if (dup2(pipefd[i][1], STDOUT_FILENO) == -1)
			error_open(arg, pipefd, &fd, 5);
	}
	else
		case_last_cmd(&fd, arg, pipefd);
	free_pipefd(pipefd, arg.size_cmd - 1);
	execute_command(arg, envp, i);
}

int	wait_child(t_cmd arg, pid_t *pids)
{
	int	status;
	int	i;
	int	exit_num;

	i = -1;
	exit_num = 1;
	while (++i < arg.size_cmd)
	{
		if (waitpid(pids[i], &status, 0) > 0)
			if (WIFEXITED(status))
				exit_num = WEXITSTATUS(status);
	}
	return (exit_num);
}
