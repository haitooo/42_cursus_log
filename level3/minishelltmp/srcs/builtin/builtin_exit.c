/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:25:47 by hito              #+#    #+#             */
/*   Updated: 2025/03/26 03:01:42 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_exit_child(t_tokens **tokens, t_var **varlist, char *tmp)
{
	free_tokens(tokens);
	free_varlist(varlist);
	if (tmp)
		free(tmp);
}

int	builtin_exit_child(t_tokens **tokens, t_var **varlist)
{
	int			exit_code;
	t_tokens	*token;
	char		*tmp;

	token = *tokens;
	if (!token->next)
	{
		free_exit_child(tokens, varlist, NULL);
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
		free_exit_child(tokens, varlist, NULL);
		exit(0);
	}
	tmp = trim_edges_space(token->next->token);
	if (!tmp)
		return (error_node(ERRNO_ONE), FAILED);
	else if (token->next && ft_strcmp(tmp, "--") != 0 && !is_numeric_argument(tmp))
	{
		ft_eprintf("minishell: exit: %s: numeric argument required\n",
			tmp);
		free_exit_child(tokens, varlist, tmp);
		exit(2);
	}
	if (token->next && token->next->next)
		return (ft_eprintf("minishell: exit: too many arguments\n"), FAILED);
	if (token->next)
	{
		if (!is_numeric_argument(tmp))
		{
			ft_eprintf("minishell: exit: %s: numeric argument required\n",
				tmp);
			free_exit_child(tokens, varlist, tmp);
			exit(2);
		}
		exit_code = ft_atoi_exit(tmp) % 256;
		free_exit_child(tokens, varlist, tmp);
		exit(exit_code);
	}
	exit_code = get_exit_status(varlist);
	free_exit_child(tokens, varlist, tmp);
	return (exit(exit_code), exit_code);
}

static void	free_exit_re(char **in, t_status **st_head, t_var **varlist, char *tmp)
{
	free(*in);
	frees(*st_head, varlist);
	if (tmp)
		free(tmp);
}

int	builtin_exit_re(t_tokens **tokens, t_var **varlist, t_status *st_head,
		char *in)
{
	int			exit_code;
	t_tokens	*token;
	char		*tmp;

	token = *tokens;
	if (!token->next)
	{
		free_exit_re(&in, &st_head, varlist, NULL);
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
		free_exit_child(tokens, varlist, NULL);
		exit(0);
	}
	tmp = trim_edges_space(token->next->token);
	if (!tmp)
		return (error_node(ERRNO_ONE), FAILED);
	else if (token->next && ft_strcmp(tmp, "--") != 0 && !is_numeric_argument(tmp))
	{
		ft_eprintf("minishell: exit: %s: numeric argument required\n",
			tmp);
		free_exit_re(&in, &st_head, varlist, tmp);
		exit(2);
	}
	if (token->next && token->next->next)
		return (ft_eprintf("minishell: exit: too many arguments\n"), 1);
	if (token->next)
	{
		if (!is_numeric_argument(tmp))
		{
			ft_eprintf("minishell: exit: %s: numeric argument required\n",
				tmp);
			free_exit_re(&in, &st_head, varlist, tmp);
			exit(2);
		}
		exit_code = ft_atoi_exit(tmp) % 256;
		free_exit_re(&in, &st_head, varlist, tmp);
		exit(exit_code);
	}
	exit_code = get_exit_status(varlist);
	free_exit_re(&in, &st_head, varlist, tmp);
	return (exit(exit_code), exit_code);
}

int	builtin_exit(t_tokens **tokens, t_var **varlist, t_status *st_head)
{
	int			exit_code;
	t_tokens	*token;
	char		*tmp;

	token = *tokens;
	printf("exit\n");
	if (!token->next)
	{
		frees(st_head, varlist);
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
		free_exit_child(tokens, varlist, NULL);
		exit(0);
	}
	tmp = trim_edges_space(token->next->token);
	if (!tmp)
		return (error_node(ERRNO_ONE), FAILED);
	else if (token->next && ft_strcmp(tmp, "--") != 0 && !is_numeric_argument(tmp))
	{
		ft_eprintf("minishell: exit: %s: numeric argument required\n",
			tmp);
		frees(st_head, varlist);
		free(tmp);
		exit(2);
	}
	if (token->next && token->next->next)
		return (ft_eprintf("minishell: exit: too many arguments\n"), 1);
	if (token->next)
	{
		if (!is_numeric_argument(tmp))
		{
			ft_eprintf("minishell: exit: %s: numeric argument required\n",
				tmp);
			frees(st_head, varlist);
			free(tmp);
			exit(2);
		}
		exit_code = ft_atoi_exit(tmp) % 256;
		frees(st_head, varlist);
		free(tmp);
		exit(exit_code);
	}
	exit_code = get_exit_status(varlist);
	frees(st_head, varlist);
	free(tmp);
	return (exit(exit_code), exit_code);
}
