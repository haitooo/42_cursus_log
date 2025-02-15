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

//void	close_pipefd(int **pipefd, int i)
//{
//	if (!pipefd)
//		return ;
//	while (--i >= 0)
//	{
//		close(pipefd[i][0]);
//		close(pipefd[i][1]);
//	}
//}

void	after_call_child(t_cmd arg, int **pipefd)
{
	int	n;

	n = 0;
	free_pipefd(pipefd, arg.size_cmd - 1);
	while (n < arg.size_cmd)
		free(arg.path[n++]);
	free(arg.path);
	if (ft_strcmp(arg.infile_name, "here_doc") == 0)
		unlink("here_doc");
}
