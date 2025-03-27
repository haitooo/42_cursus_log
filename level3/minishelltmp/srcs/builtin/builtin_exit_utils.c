/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 11:16:32 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/24 11:17:07 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_numeric_argument(const char *arg)
{
	int	i;

	i = 0;
	if (!arg || !arg[i])
		return (0);
	if (arg[i] == '+' || arg[i] == '-')
	{
		i++;
		if (!arg[i])
			return (0);
	}
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

int	get_exit_status(t_var **varlist)
{
	t_var	*var;
	int		exit_status;

	var = *varlist;
	while (var && ft_strcmp(var->name, "?") != 0)
		var = var->next;
	if (!var)
		return (ft_eprintf("minishell: exit: not found exit status\n"), 1);
	exit_status = ft_atoi(var->value) % 256;
	return (exit_status);
}

void	handle_exit_nonop(t_tokens **tokens)
{
	t_tokens	*token;
	t_tokens	*nonop;

	token = *tokens;
	nonop = token->next;
	token->next = token->next->next;
	free(nonop->token);
	free(nonop);
}
