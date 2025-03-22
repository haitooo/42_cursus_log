/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 23:08:55 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/22 04:18:28 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_var(t_var *var)
{
	free(var->name);
	free(var->value);
	free(var);
}

static t_var	*remove_var(t_var **varlist, char *name)
{
	t_var	*head;
	t_var	*tmp_var;
	t_var	*remove_var;

	if (!varlist && !*varlist)
		return (NULL);
	head = *varlist;
	tmp_var = *varlist;
	if (!ft_strcmp(tmp_var->name, name))
	{
		head = tmp_var->next;
		free_var(tmp_var);
		return (head);
	}
	while (tmp_var->next && ft_strcmp(tmp_var->next->name, name))
		tmp_var = tmp_var->next;
	if (!tmp_var->next)
		return (head);
	remove_var = tmp_var->next;
	tmp_var->next = remove_var->next;
	free_var(remove_var);
	return (head);
}

int	builtin_unset(t_tokens **tokens, t_var **varlist)
{
	t_tokens	*head;

	head = (*tokens)->next;
	if (!head || !varlist || !*varlist)
		return (EXIT_SUCCESS);
	while (head)
	{
		if (ft_strcmp(head->token, "?"))
			*varlist = remove_var(varlist, head->token);
		head = head->next;
	}
	return (EXIT_SUCCESS);
}
