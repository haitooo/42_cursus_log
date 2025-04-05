/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:25:47 by hito              #+#    #+#             */
/*   Updated: 2025/04/04 00:38:05 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_exit_(t_status *st_head, t_var **varlist, char *tmp,
		int exit_code)
{
	frees(st_head, varlist);
	if (tmp)
		free(tmp);
	exit(exit_code);
}

static void	free_exit(t_status *st_head, t_var **varlist, char *tmp,
		int exit_code)
{
	frees(st_head, varlist);
	if (tmp)
		free(tmp);
	exit(exit_code);
}

int	check_numeric_exit(t_tokens *token, t_var **varlist, t_status *st_head,
		char *tmp)
{
	int		exit_code;

	tmp = trim_edges_space(token->next->token);
	if (!tmp)
		return (error_node(ERRNO_ONE), FAILED);
	if (token->next && ft_strcmp(tmp, "--") != 0 && !is_numeric_argument(tmp))
	{
		ft_eprintf("minishell: exit: %s: numeric argument required\n", tmp);
		free_exit(st_head, varlist, tmp, 2);
	}
	if (token->next && token->next->next)
		return (free(tmp), ft_eprintf
			("minishell: exit: too many arguments\n"), 1);
	if (token->next)
	{
		if (!is_numeric_argument(tmp))
		{
			ft_eprintf("minishell: exit: %s: numeric argument required\n", tmp);
			free_exit(st_head, varlist, tmp, 2);
		}
		exit_code = ft_atoi_exit(tmp) % 256;
		free_exit_(st_head, varlist, tmp, exit_code);
	}
	free(tmp);
	return (0);
}

int	builtin_exit(t_tokens **tokens, t_var **varlist, t_status *st_head)
{
	int			exit_code;
	t_tokens	*token;
	char		*tmp;

	token = *tokens;
	printf("exit\n");
	if (!token->next)
		free_exit(st_head, varlist, NULL, 0);
	tmp = trim_edges_space(token->next->token);
	if (!tmp)
		return (error_node(ERRNO_ONE), FAILED);
	if (token->next && ft_strcmp(tmp, "--") == 0)
		handle_exit_nonop(tokens);
	free(tmp);
	if (!token->next)
		free_exit(st_head, varlist, NULL, 0);
	if (check_numeric_exit(token, varlist, st_head, NULL))
		return (FAILED);
	exit_code = get_exit_status(varlist);
	free_exit(st_head, varlist, NULL, exit_code);
	return (exit_code);
}
