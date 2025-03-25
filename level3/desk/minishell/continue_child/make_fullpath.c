/* ************************************************************************** */ /*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_fullpath.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 22:00:53 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/22 17:03:14 by tssaito          ###   ########.fr       */
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
	if (access(fullpath, F_OK))
	{
		free(fullpath);
		exit_child(child, EXIT_NOCMD, ENOENT, cmd);
	}
	if (!stat(fullpath, &path_stat) && S_ISDIR(path_stat.st_mode))
	{
		free(fullpath);
		exit_child(child, EXIT_PERM, EISDIR, cmd);
	}
	if (access(fullpath, X_OK))
	{
		free(fullpath);
		exit_child(child, EXIT_PERM, EACCES, cmd);
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
		exit_child(child, EXIT_FAILURE, errno, "malloc error");
	if (access(fullpath, F_OK))
		exit_child(child, EXIT_NOCMD, ENOENT, cmd);
	if (!stat(fullpath, &path_stat) && S_ISDIR(path_stat.st_mode))
		exit_child(child, EXIT_PERM, EISDIR, fullpath);
	if (access(fullpath, X_OK))
		exit_child(child, EXIT_PERM, EACCES, cmd);
	return (fullpath);
}

static char	*concat_path_with_cmd(t_child *child, char *cmd, t_var **varlist)
{
	t_var	*path_var;
	char	*fullpath;
	char	*saved_path;
	int		i;

	path_var = get_var(varlist, "PATH");
	child->paths = ft_split(path_var->value, ':');
	if (!child->paths)
		exit_child(child, EXIT_FAILURE, errno, "malloc error");
	i = 0;
	saved_path = NULL;
	while (child->paths[i])
	{
		fullpath = ft_strjoin_three(child->paths[i], "/", cmd);
		if (!fullpath)
			exit_child(child, EXIT_FAILURE, errno, "malloc error");
		if (!access(fullpath, X_OK))
			break ;
		if(!access(fullpath, F_OK) && !saved_path)
			saved_path = fullpath;
		else
			free(fullpath);
		fullpath = NULL;
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
	if (!cmd || !cmd[0])
		return ;
	if (cmd[0] == '/' || !ft_strncmp(cmd, "./", 2))
		child->fullpath = check_original_path(child, cmd);
	else if (varlist && !ft_strncmp(cmd, "~/", 2))
		child->fullpath = concat_home_with_cmd(child, cmd, varlist);
	else if (get_var(varlist, "PATH"))
		child->fullpath = concat_path_with_cmd(child, cmd, varlist);
	else
		child->fullpath = check_original_path(child, cmd);
}
