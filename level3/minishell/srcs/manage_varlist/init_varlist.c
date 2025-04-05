/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_varlist.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 14:21:12 by tssaito           #+#    #+#             */
/*   Updated: 2025/04/04 00:28:10 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exit_varlist(t_var **varlist, char *errmsg)
{
	free_varlist(varlist);
	ft_putendl_fd(errmsg, STDERR_FILENO);
}

static void	split_var(t_var **varlist, char *envp, char **name, char **value)
{
	int	name_size;

	name_size = 0;
	*name = NULL;
	*value = NULL;
	while (envp[name_size] && envp[name_size] != '=')
		name_size++;
	*name = (char *)malloc(sizeof(char) * (name_size + 1));
	if (!*name)
		exit_varlist(varlist, strerror(errno));
	if (!envp[name_size] || !envp[name_size + 1])
	{
		*value = NULL;
		free(*name);
		*name = NULL;
		return ;
	}
	ft_strlcpy(*name, envp, name_size + 1);
	*value = ft_strdup(&envp[name_size + 1]);
	if (!*value)
	{
		free(*name);
		*name = NULL;
		exit_varlist(varlist, strerror(errno));
	}
}

t_var	*init_varlist(char **envp, char *c1, char *c2)
{
	t_var	*varlist;
	int		var_size;
	char	*var_name;
	char	*var_value;

	if (!c1 || !c2)
		error_node(ERRNO_ONE);
	if (!envp)
		ft_eprintf("minishell: cannot get envp\n");
	if (!c1 || !c2 || !envp)
		exit(1);
	varlist = NULL;
	var_size = 0;
	while (envp[var_size])
	{
		split_var(&varlist, envp[var_size], &var_name, &var_value);
		if (var_name)
			add_var(&varlist, var_name, var_value);
		var_size++;
	}
	add_var(&varlist, c1, c2);
	return (varlist);
}
