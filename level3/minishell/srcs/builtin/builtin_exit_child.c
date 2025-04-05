/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit_child.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 05:57:49 by haito             #+#    #+#             */
/*   Updated: 2025/03/31 06:43:57 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_exit_child(t_tokens *token, t_var **varlist, char *tmp)
{
	free_tokens(&token);
	free_varlist(varlist);
	if (tmp)
		free(tmp);
}

void	error_numeric_exit_child(t_tokens *token, t_var **varlist, char *tmp)
{
	ft_eprintf("minishell: exit: %s: numeric argument required\n",
		tmp);
	free_exit_child(token, varlist, tmp);
	exit(2);
}

int	check_numeric_exit_child(t_tokens *token, t_var **varlist,
		char *tmp)
{
	int		exit_code;

	tmp = trim_edges_space(token->next->token);
	if (!tmp)
		return (error_node(ERRNO_ONE), FAILED);
	if (token->next && ft_strcmp(tmp, "--") != 0 && !is_numeric_argument(tmp))
		error_numeric_exit_child(token, varlist, tmp);
	if (token->next && token->next->next)
		return (free(tmp), ft_eprintf
			("minishell: exit: too many arguments\n"), FAILED);
	if (token->next)
	{
		if (!is_numeric_argument(tmp))
			error_numeric_exit_child(token, varlist, tmp);
		exit_code = ft_atoi_exit(tmp) % 256;
		free_exit_child(token, varlist, tmp);
		exit(exit_code);
	}
	return (0);
}

int	builtin_exit_child(t_tokens **tokens, t_var **varlist)
{
	int			exit_code;
	t_tokens	*token;
	char		*tmp;

	token = *tokens;
	if (!token->next)
	{
		free_exit_child(token, varlist, NULL);
		exit(0);
	}
	tmp = trim_edges_space(token->next->token);
	if (!tmp)
		return (error_node(ERRNO_ONE), FAILED);
	if (token->next && ft_strcmp(tmp, "--") == 0)
		handle_exit_nonop(tokens);
	free(tmp);
	if (!token->next)
	{
		free_exit_child(token, varlist, NULL);
		exit(0);
	}
	check_numeric_exit_child(token, varlist, NULL);
	exit_code = get_exit_status(varlist);
	free_exit_child(token, varlist, NULL);
	return (exit(exit_code), exit_code);
}
