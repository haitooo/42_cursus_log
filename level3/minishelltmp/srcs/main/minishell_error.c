/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 23:32:04 by haito             #+#    #+#             */
/*   Updated: 2025/03/26 01:34:14 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*error_add_char(int *i, char *cmds)
{
	if (cmds)
		free(cmds);
	if (i)
		*i = ERROR;
	return (NULL);
}

int	error_handle_brackets(int error_num, t_var **var)
{
	if (error_num == ERRNO_ONE)
	{
		update_exit_code(SYNERR, var);
		ft_eprintf("minishell: syntax error near unexpected token `('\n");
	}
	if (error_num == ERRNO_TWO)
	{
		update_exit_code(SYNERR, var);
		ft_eprintf("minishell: syntax error near unexpected token `)'\n");
	}
	if (error_num == ERRNO_THREE)
	{
		update_exit_code(FAILED, var);
		perror("minishell: malloc error: ");
	}
	if (error_num == ERRNO_FOUR)
	{
		update_exit_code(SYNERR, var);
		ft_eprintf("minishell: syntax error after `)'\n");
	}
	return (ERROR);
}

int	error_node(int error_num)
{
	if (error_num == ERRNO_ONE)
		ft_eprintf("minishell: malloc error: Cannot allocate memory\n");
	return (ERROR);
}

int	error_pipe(int error_num, t_var **var, int ope, char *cmds)
{
	char	*cmd;

	if (ope == IS_AND)
		cmd = ft_strdup("&");
	else if (ope == IS_ANDAND)
		cmd = ft_strdup("&&");
	else if (ope == IS_PIPE)
		cmd = ft_strdup("|");
	else if (ope == IS_OR)
		cmd = ft_strdup("||");
	else if (ope == IS_SEMI)
		cmd = ft_strdup(";");
	else if (ope == IS_CMD)
		cmd = ft_strdup(cmds);
	if (error_num == ERRNO_ONE)
	{
		update_exit_code(SYNERR, var);
		ft_eprintf("minishell: syntax error near unexpected token `%s'\n", cmd);
	}
	if (error_num == ERRNO_TWO)
	{
		update_exit_code(FAILED, var);
		perror("minishell: pipe error");
	}
	return (free(cmd), ERROR);
}
