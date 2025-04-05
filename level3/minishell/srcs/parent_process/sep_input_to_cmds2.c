/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sep_input_to_cmds2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 03:11:52 by haito             #+#    #+#             */
/*   Updated: 2025/03/31 03:12:37 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	skip_spaces(const char *input, int i)
{
	if (i == ERROR)
		return (ERROR);
	while (input[i] == ' ')
		i++;
	i--;
	return (i);
}

int	handle_quotes(const char *input, char **cmds, int i, t_var **var)
{
	char	quote;

	quote = input[i];
	*cmds = add_char(*cmds, input[i++], &i, var);
	if (!*cmds)
		return (update_exit_code(FAILED, var), ERROR);
	while (input[i] && input[i] != quote)
		*cmds = add_char(*cmds, input[i++], &i, var);
	if (!*cmds)
		return (update_exit_code(FAILED, var), ERROR);
	*cmds = add_char(*cmds, input[i], &i, var);
	if (!*cmds)
		return (update_exit_code(FAILED, var), ERROR);
	return (i);
}

int	handle_brackets(const char *input, t_brackets *brackets,
	t_status **st_head, t_parser *ps)
{
	int			pair;
	t_status	*new_node;

	if (ps->cmds)
		return (error_handle_brackets(ERRNO_ONE, ps->var));
	pair = get_brackets_pair(ps->i, brackets);
	if (pair - ps->i <= 1)
		return (error_handle_brackets(ERRNO_TWO, ps->var));
	ps->cmds = trim_spaces(ft_substr(input, ps->i + 1, pair - ps->i - 1));
	if (!ps->cmds)
		return (error_handle_brackets(ERRNO_THREE, ps->var));
	new_node = ft_new_node(ps->cmds, 1);
	if (!new_node)
		return (error_handle_brackets(ERRNO_THREE, ps->var));
	ft_add_back_node(st_head, new_node);
	free(ps->cmds);
	ps->cmds = NULL;
	ps->i = pair;
	while (input[ps->i + 1] && input[ps->i + 1] == ' ')
		ps->i++;
	if ((input[ps->i + 1] != '\0' && input[ps->i + 1] != '|'
			&& input[ps->i + 1] != '&' && input[ps->i + 1] != ';')
		|| (input[ps->i + 1] == '&' && input[ps->i + 2] != '&'))
		return (error_handle_brackets(ERRNO_FOUR, ps->var));
	return (ps->i);
}
