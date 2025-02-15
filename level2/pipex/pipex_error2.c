#include "pipex.h"

void	error_open(t_cmd arg, int **pipefd, int *fd, int errornum)
{
	int	n;

	n = 0;
	if (errornum == 1)
		perror("Error opening infile");
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

void	path_free(t_cmd arg)
{
	int	n;

	n = 0;
	while (n <= arg.size_cmd)
		free(arg.path[n++]);
	free(arg.path);
}
