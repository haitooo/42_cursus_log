/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 22:09:13 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/15 21:41:45 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_var	*get_var(t_var **varlist, char *name)
{
	t_var	*tmp_var;

	if (!varlist || !*varlist)
		return (NULL);
	tmp_var = *varlist;
	while (tmp_var && ft_strcmp(tmp_var->name, name))
		tmp_var = tmp_var->next;
	return (tmp_var);
}

static t_exist	replace_existent_var(t_var **varlist, char *name, char *value)
{
	t_var	*existent_var;

	existent_var = get_var(varlist, name);
	if (!existent_var)
		return (NONE);
	free(existent_var->value);
	existent_var->value = value;
	free(name);
	return (EXIST);
}

void	add_var(t_var **varlist, char *name, char *value)
{
	t_var	*new_var;
	t_var	*backward_var;

	if (replace_existent_var(varlist, name, value) == EXIST)
		return ;
	new_var = (t_var *)malloc(sizeof(t_var));
	if (!new_var)
	{
		free(name);
		free(value);
		free_varlist(varlist);
	}
	new_var->name = name;
	new_var->value = value;
	new_var->next = NULL;
	if (!*varlist)
	{
		*varlist = new_var;
		return ;
	}
	backward_var = *varlist;
	while (backward_var && backward_var->next)
		backward_var = backward_var->next;
	backward_var->next = new_var;
}

t_var	**remove_var(t_var **varlist, char *name)
{
	t_var	**head;
	t_var	*tmp_var;
	t_var	*remove_var;

	if (!varlist && !*varlist)
		return (NULL);
	head = varlist;
	tmp_var = *varlist;
	if (!ft_strcmp(tmp_var->name, name))
	{
		head = &tmp_var->next;
		free(tmp_var);
		return (head);
	}
	while (tmp_var->next && ft_strcmp(tmp_var->next->name, name))
		tmp_var = tmp_var->next;
	if (!tmp_var->next)
		return (head);
	remove_var = tmp_var->next;
	tmp_var->next = remove_var->next;
	free(remove_var->name);
	free(remove_var->value);
	free(remove_var);
	return (head);
}
