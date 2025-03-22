/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:25:47 by hito              #+#    #+#             */
/*   Updated: 2025/03/22 08:13:00 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_numeric_argument(const char *arg)
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

int	builtin_exit(t_tokens **tokens, t_var **varlist, t_status *st_head)
{
	int			exit_code;
	t_tokens	*token;

	token = *tokens;
	printf("exit\n");
	if (token->next && token->next->next)
		return (ft_eprintf("minishell: exit: too many arguments\n"), 1);
	if (token->next)
	{
		if (!is_numeric_argument(token->next->token))
		{
			ft_eprintf("minishell: exit: %s: numeric argument required\n", token->token);
			frees(st_head, varlist);
			exit(2);
		}
		exit_code = ft_atoi(token->next->token) % 256;
		frees(st_head, varlist);
		exit(exit_code);
	}
	exit_code = get_exit_status(varlist);
	frees(st_head, varlist);
	return (exit(exit_code), exit_code);
}
