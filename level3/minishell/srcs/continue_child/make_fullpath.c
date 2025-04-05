/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_fullpath.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 23:25:36 by tssaito           #+#    #+#             */
/*   Updated: 2025/04/03 16:45:19 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*check_original_path(t_child *child, char *cmd)
{
	char		*fullpath;
	struct stat	path_stat;

	fullpath = ft_strdup(cmd);
	if (!fullpath)
		exit_child(child, EXIT_FAILURE, errno, "malloc error");
	else
	{
		if (access(fullpath, F_OK))
		{
			free(fullpath);
			return (exit_child(child, EXIT_NOCMD, ENOENT, cmd), NULL);
		}
		else if (!stat(fullpath, &path_stat) && S_ISDIR(path_stat.st_mode))
		{
			free(fullpath);
			return (exit_child(child, EXIT_PERM, EISDIR, cmd), NULL);
		}
		else if (access(fullpath, X_OK))
		{
			free(fullpath);
			return (exit_child(child, EXIT_PERM, EACCES, cmd), NULL);
		}
	}
	return (fullpath);
}

static char	*concat_home_with_cmd(t_child *child, char *cmd, t_var **varlist)
{
	struct stat	path_stat;
	char		*fullpath;
	t_var		*home;

	home = get_var(varlist, "HOME");
	if (home)
		fullpath = ft_strjoin(home->value, &cmd[1]);
	else
		fullpath = ft_strdup(cmd);
	if (!fullpath)
		return (exit_child(child, EXIT_FAILURE, errno, "malloc error"), NULL);
	if (access(fullpath, F_OK))
		exit_child(child, EXIT_NOCMD, ENOENT, cmd);
	if (!stat(fullpath, &path_stat) && S_ISDIR(path_stat.st_mode))
		exit_child(child, EXIT_PERM, EISDIR, fullpath);
	if (access(fullpath, X_OK))
		exit_child(child, EXIT_PERM, EACCES, cmd);
	return (fullpath);
}

static char	*get_fullpath(t_child *child, char *path, char *cmd, char **saved)
{
	char	*fullpath;

	fullpath = ft_strjoin_three(path, "/", cmd);
	if (!fullpath)
		return (exit_child(child, EXIT_FAILURE, errno, "malloc error"), NULL);
	if (!access(fullpath, X_OK))
		return (fullpath);
	if (!access(fullpath, F_OK) && !*saved)
		*saved = fullpath;
	else
		free(fullpath);
	fullpath = NULL;
	return (fullpath);
}

static char	*concat_path_with_cmd(t_child *child, char *cmd, t_var **varlist)
{
	t_var	*path_var;
	char	*fullpath;
	char	*saved_path;
	int		i;

	fullpath = NULL;
	path_var = get_var(varlist, "PATH");
	saved_path = NULL;
	child->paths = ft_split(path_var->value, ':');
	if (!child->paths)
		exit_child(child, EXIT_FAILURE, errno, "malloc error");
	i = 0;
	while (child->paths[i])
	{
		fullpath = get_fullpath(child, child->paths[i], cmd, &saved_path);
		if (fullpath)
			break ;
		i++;
	}
	if (!fullpath && saved_path)
		exit_child(child, EXIT_PERM, PATHERROR, saved_path);
	else if (!fullpath)
		exit_child(child, EXIT_NOCMD, CMDERROR, cmd);
	return (fullpath);
}

void	make_fullpath(t_child *child, char *cmd, t_var **varlist)
{
	t_var	*var;

	if (!cmd)
		return ;
	if (!cmd[0])
		exit_child(child, EXIT_NOCMD, CMDERROR, cmd);
	if (cmd[0] == '/' || !ft_strncmp(cmd, "./", 2))
		child->fullpath = check_original_path(child, cmd);
	else if (varlist && !ft_strncmp(cmd, "~/", 2))
		child->fullpath = concat_home_with_cmd(child, cmd, varlist);
	else
	{
		var = get_var(varlist, "PATH");
		if (var && var->value)
			child->fullpath = concat_path_with_cmd(child, cmd, varlist);
		else
			child->fullpath = check_original_path(child, cmd);
	}
}
