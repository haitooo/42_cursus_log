/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 16:13:42 by hito              #+#    #+#             */
/*   Updated: 2025/04/04 01:12:45 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	update_pwd(t_var **varlist, char *cwd)
{
	char	*name_dup;
	char	*value_dup;

	name_dup = ft_strdup("PWD");
	if (!name_dup)
		return (free(cwd), error_node(ERRNO_ONE), FAILED);
	value_dup = ft_strdup(cwd);
	free(cwd);
	if (!value_dup)
		return (free(name_dup), error_node(ERRNO_ONE), FAILED);
	add_var(varlist, name_dup, value_dup);
	return (SUCCESS);
}

int	set_oldpwd(t_var **varlist, char *cwd)
{
	char	*oldpwd;
	char	*name_dup;
	char	*value_dup;

	oldpwd = get_path(varlist, "PWD");
	name_dup = ft_strdup("OLDPWD");
	if (!name_dup)
		return (free(cwd), error_node(ERRNO_ONE), FAILED);
	if (!oldpwd)
	{
		add_var(varlist, name_dup, NULL);
		return (update_pwd(varlist, cwd));
	}
	value_dup = ft_strdup(oldpwd);
	if (!value_dup)
		return (free(name_dup), free(cwd), error_node(ERRNO_ONE), FAILED);
	add_var(varlist, name_dup, value_dup);
	return (update_pwd(varlist, cwd));
}

int	update_oldpwd(t_var **varlist)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		if (errno == ENOENT)
		{
			ft_eprintf("cd: error retrieving current directory: ");
			ft_eprintf("getcwd: cannot access parent directories:");
			ft_eprintf(" No such file or directory\n");
			cwd = ft_strjoin(get_path(varlist, "PWD"), "/..");
		}
		else
			return (perror("minishell: cd"), FAILED);
	}
	return (set_oldpwd(varlist, cwd));
}

char	*get_cd_target(t_tokens *token, t_var **varlist)
{
	char	*dir;

	dir = NULL;
	if (token->next && token->next->next)
		return (ft_eprintf("minishell: cd: too many arguments\n"), NULL);
	if ((token->next && (!ft_strcmp(token->next->token, "--")
				|| !ft_strcmp(token->next->token, "~"))) || !token->next)
	{
		dir = get_path(varlist, "HOME");
		if (!dir)
			return (ft_eprintf("minishell: cd: HOME not set\n"), NULL);
	}
	else if (!ft_strcmp(token->next->token, "-"))
	{
		dir = get_path(varlist, "OLDPWD");
		if (!dir)
			return (ft_eprintf("minishell: cd: OLDPWD not set\n"), NULL);
		printf("%s\n", dir);
		return (dir);
	}
	else if (!ft_strcmp(token->next->token, ""))
		return (".");
	return (token->next->token);
}

int	builtin_cd(t_tokens **tokens, t_var **varlist)
{
	t_tokens	*token;
	char		*dir;
	int			len;

	token = *tokens;
	dir = get_cd_target(token, varlist);
	if (!dir)
		return (FAILED);
	if (access(dir, F_OK) == 0 && access(dir, X_OK) == -1)
	{
		dir = getcwd(NULL, 0);
		len = ft_strlen(dir);
		while (--len > 0)
		{
			if (dir[len] == '/')
				break ;
		}
		dir[len] = '\0';
	}
	if (chdir(dir) == ERROR)
		return (ft_eprintf
			("minishell: cd: %s: %s\n", dir, strerror(errno)), FAILED);
	return (update_oldpwd(varlist));
}
