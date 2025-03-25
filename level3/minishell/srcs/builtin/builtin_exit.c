/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:25:47 by hito              #+#    #+#             */
/*   Updated: 2025/03/24 11:21:51 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_exit_child(t_tokens **tokens, t_var **varlist)
{
	free_tokens(tokens);
	free_varlist(varlist);
}

int	builtin_exit_child(t_tokens **tokens, t_var **varlist)
{
	int			exit_code;
	t_tokens	*token;

	token = *tokens;
	if (token->next && ft_strcmp(token->next->token, "--") == 0)
		handle_exit_nonop(tokens);
	if (token->next && token->next->next)
		return (ft_eprintf("minishell: exit: too many arguments\n"), 1);
	if (token->next)
	{
		if (!is_numeric_argument(token->next->token))
		{
			ft_eprintf("minishell: exit: %s: numeric argument required\n",
				token->next->token);
			free_exit_child(tokens, varlist);
			exit(2);
		}
		exit_code = ft_atoi_exit(token->next->token) % 256;
		free_exit_child(tokens, varlist);
		exit(exit_code);
	}
	exit_code = get_exit_status(varlist);
	free_exit_child(tokens, varlist);
	return (exit(exit_code), exit_code);
}

static void	free_exit_re(char **in, t_status **st_head, t_var **varlist)
{
	free(*in);
	frees(*st_head, varlist);
}

int	builtin_exit_re(t_tokens **tokens, t_var **varlist, t_status *st_head,
		char *in)
{
	int			exit_code;
	t_tokens	*token;

	token = *tokens;
	if (token->next && ft_strcmp(token->next->token, "--") == 0)
		handle_exit_nonop(tokens);
	if (token->next && token->next->next)
		return (ft_eprintf("minishell: exit: too many arguments\n"), 1);
	if (token->next)
	{
		if (!is_numeric_argument(token->next->token))
		{
			ft_eprintf("minishell: exit: %s: numeric argument required\n",
				token->next->token);
			free_exit_re(&in, &st_head, varlist);
			exit(2);
		}
		exit_code = ft_atoi_exit(token->next->token) % 256;
		free_exit_re(&in, &st_head, varlist);
		exit(exit_code);
	}
	exit_code = get_exit_status(varlist);
	free_exit_re(&in, &st_head, varlist);
	return (exit(exit_code), exit_code);
}

int	builtin_exit(t_tokens **tokens, t_var **varlist, t_status *st_head)
{
	int			exit_code;
	t_tokens	*token;

	token = *tokens;
	printf("exit\n");
	if (token->next && ft_strcmp(token->next->token, "--") == 0)
		handle_exit_nonop(tokens);
	if (token->next && token->next->next)
		return (ft_eprintf("minishell: exit: too many arguments\n"), 1);
	if (token->next)
	{
		if (!is_numeric_argument(token->next->token))
		{
			ft_eprintf("minishell: exit: %s: numeric argument required\n",
				token->next->token);
			frees(st_head, varlist);
			exit(2);
		}
		exit_code = ft_atoi_exit(token->next->token) % 256;
		frees(st_head, varlist);
		exit(exit_code);
	}
	exit_code = get_exit_status(varlist);
	frees(st_head, varlist);
	return (exit(exit_code), exit_code);
}
