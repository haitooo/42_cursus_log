/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit_re.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 05:58:00 by haito             #+#    #+#             */
/*   Updated: 2025/03/31 07:15:35 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_exit_re(t_lp *lp, t_status **st_head, t_var **varlist,
		char *tmp)
{
	free(lp->input);
	if (lp->heredoc)
		free(lp->heredoc);
	frees(*st_head, varlist);
	if (tmp)
		free(tmp);
}

void	error_numeric_exit_re(t_var **varlist, t_status *st_head,
	t_lp *lp, char *tmp)
{
	ft_eprintf("minishell: exit: %s: numeric argument required\n",
		tmp);
	free_exit_re(lp, &st_head, varlist, tmp);
	exit(2);
}

void	nonop_exit_re(t_var **varlist, t_status *st_head,
	t_lp *lp)
{
	free_exit_re(lp, &st_head, varlist, NULL);
	exit(0);
}

int	check_numeric_exit_re(t_tokens *token, t_var **varlist, t_status *st_head,
	t_lp *lp)
{
	int		exit_code;
	char	*tmp;

	tmp = NULL;
	tmp = trim_edges_space(token->next->token);
	if (!tmp)
		return (error_node(ERRNO_ONE), FAILED);
	if (token->next && ft_strcmp(tmp, "--") != 0 && !is_numeric_argument(tmp))
		error_numeric_exit_re(varlist, st_head, lp, tmp);
	if (token->next && token->next->next)
		return (free(tmp), ft_eprintf
			("minishell: exit: too many arguments\n"), 1);
	if (token->next)
	{
		if (!is_numeric_argument(tmp))
			error_numeric_exit_re(varlist, st_head, lp, tmp);
		exit_code = ft_atoi_exit(tmp) % 256;
		free_exit_re(lp, &st_head, varlist, tmp);
		exit(exit_code);
	}
	return (0);
}

int	builtin_exit_re(t_tokens **tokens, t_var **varlist, t_status *st_head,
		t_lp *lp)
{
	int			exit_code;
	t_tokens	*token;
	char		*tmp;

	token = *tokens;
	if (!token->next)
		nonop_exit_re(varlist, st_head, lp);
	tmp = trim_edges_space(token->next->token);
	if (!tmp)
		return (error_node(ERRNO_ONE), FAILED);
	if (token->next && ft_strcmp(tmp, "--") == 0)
		handle_exit_nonop(tokens);
	free(tmp);
	if (!token->next)
		nonop_exit_re(varlist, st_head, lp);
	if (check_numeric_exit_re(token, varlist, st_head, lp))
		return (FAILED);
	exit_code = get_exit_status(varlist);
	free_exit_re(lp, &st_head, varlist, NULL);
	return (exit(exit_code), exit_code);
}
