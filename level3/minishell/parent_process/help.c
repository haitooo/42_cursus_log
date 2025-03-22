/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 03:44:48 by haito             #+#    #+#             */
/*   Updated: 2025/03/22 04:15:52 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_cmds(t_status **st_head, t_var **varlist)
{
	t_status	*st;

	if (!st_head || !*st_head)
		return (ERROR);
	st = *st_head;
	while (st)
	{
		st->token = expander(st->cmds, varlist);
		if (!st->token)
			return (ERROR);
		st = st->next;
	}
	return (SUCCESS);
}

int	check_built_in(t_status **st_head, t_status *st)
{
	char		**builtin_cmds;
	int			i;

	if (!st_head || !*st_head || !st)
		return (ERROR);
	if (!st->token)
		return (SUCCESS);
	builtin_cmds = init_builtin_cmds();
	if (!builtin_cmds)
		return (error_node(ERRNO_ONE), ERROR);
	while (st)
	{
		i = -1;
		while (builtin_cmds[++i])
		{
			if (!ft_strcmp(st->token->token, builtin_cmds[i]))
			{
				st->is_builtin = 1;
				break ;
			}
		}
		st = st->next;
	}
	free_builtin_cmds(builtin_cmds);
	return (SUCCESS);
}

int	make_shlvl(t_var **varlist)
{
	char	*name_dup;
	char	*value_dup;

	name_dup = ft_strdup("SHLVL");
	if (!name_dup)
	{
		error_node(ERRNO_ONE);
		free_varlist(varlist);
		exit(1);
	}
	value_dup = ft_strdup("1");
	if (!value_dup)
	{
		error_node(ERRNO_ONE);
		free_varlist(varlist);
		exit(1);
	}
	add_var(varlist, name_dup, value_dup);
	return (0);
}

int	check_shlvl(t_var **varlist, t_var *var)
{
	int		i;
	int		shlvl;
	int		len;

	if (!var || !var->value)
		return (make_shlvl(varlist));
	if (var->value)
		len = ft_strlen(var->value);
	i = 0;
	while (i < len)
	{
		if (!ft_isdigit(var->value[i]))
			return (make_shlvl(varlist));
		i++;
	}
	shlvl = ft_atoi(var->value);
	free(var->value);
	var->value = ft_itoa(++shlvl);
	return (0);
}

int	count_up_shlvl(t_var **varlist)
{
	t_var	*var;

	var = *varlist;
	while (var)
	{
		if (ft_strcmp(var->name, "SHLVL") == 0)
			break ;
		var = var->next;
	}
	if (var && ft_strcmp(var->name, "SHLVL") == 0)
	{
		check_shlvl(varlist, var);
		return (0);
	}
	return (make_shlvl(varlist));
}
