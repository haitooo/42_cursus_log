/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 12:39:02 by haito             #+#    #+#             */
/*   Updated: 2025/04/04 01:26:01 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_builtin_cmds2(char **cmds)
{
	cmds[4] = ft_strdup("unset");
	if (!cmds[4])
		return (ERROR);
	cmds[5] = ft_strdup("env");
	if (!cmds[5])
		return (ERROR);
	cmds[6] = ft_strdup("exit");
	if (!cmds[6])
		return (ERROR);
	return (SUCCESS);
}

char	**init_builtin_cmds(void)
{
	char	**cmds;
	int		i;

	cmds = malloc(sizeof(char *) * 8);
	if (!cmds)
		return (NULL);
	i = 0;
	while (i < 8)
		cmds[i++] = NULL;
	cmds[0] = ft_strdup("echo");
	if (!cmds[0])
		return (free_builtin_cmds(cmds), NULL);
	cmds[1] = ft_strdup("cd");
	if (!cmds[1])
		return (free_builtin_cmds(cmds), NULL);
	cmds[2] = ft_strdup("pwd");
	if (!cmds[2])
		return (free_builtin_cmds(cmds), NULL);
	cmds[3] = ft_strdup("export");
	if (!cmds[3])
		return (free_builtin_cmds(cmds), NULL);
	if (init_builtin_cmds2(cmds) == ERROR)
		return (free_builtin_cmds(cmds), NULL);
	return (cmds);
}

void	init_ps(t_parser *ps, const char *input, t_var **var)
{
	ps->i = -1;
	ps->j = 1;
	ps->cmds = NULL;
	ps->var = var;
	ps->input = input;
}
