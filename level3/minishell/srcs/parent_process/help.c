/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 03:44:48 by haito             #+#    #+#             */
/*   Updated: 2025/03/31 02:13:51 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	update_exit_code(int exit_code, t_var **varlist)
{
	t_var	*var;

	if (!varlist || !*varlist)
		return (ft_eprintf("minishell: not found exit status\n"), ERROR);
	var = *varlist;
	while (var && ft_strcmp(var->name, "?") != 0)
		var = var->next;
	if (!var)
		return (ft_eprintf("minishell: not found exit status\n"), ERROR);
	free(var->value);
	var->value = ft_itoa(exit_code);
	if (!var->value)
		return (error_node(ERRNO_ONE));
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
	int	i;
	int	shlvl;
	int	len;

	if (!var || !var->value)
		return (make_shlvl(varlist));
	len = ft_strlen(var->value);
	if (len > 3)
		return (make_shlvl(varlist));
	i = 0;
	while (i < len)
	{
		if (!ft_isdigit(var->value[i]))
			return (make_shlvl(varlist));
		i++;
	}
	shlvl = ft_atoi(var->value);
	if (shlvl < 0 || shlvl >= 999)
		return (make_shlvl(varlist));
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
