#include "pipex.h"

int	make_path(t_cmd arg, t_param prm, char **path)
{
	int	count;

	count = -1;
	path[prm.n] = malloc(sizeof(char) * (prm.size + prm.size_cmd_len + 1));
	if (!path[prm.n])
		return (1);
	while (++count < prm.size)
		path[prm.n][count] = arg.env_path[prm.j + count];
	path[prm.n][count] = '/';
	while (++count < prm.size + prm.size_cmd_len)
		path[prm.n][count] = arg.cmds[prm.n][count - prm.size - 1];
	path[prm.n][count] = '\0';
	return (0);
}

char	**find_path(t_cmd arg, char **path)
{
	t_param	prm;

	prm.n = -1;
	while (++prm.n < arg.size_cmd)
	{
		prm.j = 0;
		while (arg.env_path[prm.j] != '\0')
		{
			prm.size = ft_strlen_sep(arg.env_path + prm.j, ':');
			prm.size_cmd_len = ft_strlen_sep(arg.cmds[prm.n], ' ') + 1;
			if (make_path(arg, prm, path) == 1)
				error_in_get_path(path, 2, prm.n);
			if (access(path[prm.n], X_OK) == 0)
				break ;
			else
			{
				free(path[prm.n]);
				path[prm.n] = NULL;
			}
			prm.j += prm.size + 1;
		}
	}
	return (path);
}

char	**get_path(t_cmd arg, char **envp)
{
	int		i;
	char	**path;

	path = malloc(sizeof(char *) * (arg.size_cmd + 1));
	if (!path)
		return (NULL);
	i = 0;
	while (envp[i] != NULL && ft_strncmp(envp[i], "PATH=", 5) != 0)
		i++;
	if (envp[i] == NULL)
		return (error_in_get_path(path, 1, 0));
	arg.env_path = envp[i] + 5;
	if (!find_path(arg, path))
		return (NULL);
	path[arg.size_cmd] = NULL;
	return (path);
}
