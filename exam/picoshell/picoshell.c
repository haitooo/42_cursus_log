#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int	picoshell(char **cmds[])
{
	if (!cmds || !cmds[0])
		return (1);
	int	size_cmds;
	while (cmds[size_cmds])
		size_cmds++;
	int		pipefd[2];
	int		input_fd;
	pid_t	pid;
	int	i = 0;

	while (i < size_cmds)
	{
		if (i != size_cmds - 1)
		{
			if (pipe(pipefd) == -1)
				return (1);
		}
		pid = fork();
		if (pid == -1)
			return (1);
		if (pid == 0)
		{
			if (i != 0)
				dup2(input_fd, STDIN_FILENO);
			if (i != size_cmds -1)
				dup2(pipefd[1], STDOUT_FILENO);
			if (i != 0)
				close(input_fd);
			close(pipefd[0]);
			close(pipefd[1]);
			execvp(cmds[i][0], cmds[i]);
			exit(1);
		}
		if (i != 0)
			close(input_fd);
		if (i != size_cmds -1)
		{
			input_fd = pipefd[0];
			close(pipefd[1]);
		}
		i++;
	}
	int	status;
	i = 0;
	while (i < size_cmds)
	{
		wait(&status);
		i++;
	}
	return (0);
}
