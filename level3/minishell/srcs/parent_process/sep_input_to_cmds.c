/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sep_input_to_cmds.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 10:54:32 by haito             #+#    #+#             */
/*   Updated: 2025/03/24 11:26:29 by tssaito          ###   ########.fr       */
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
			&& input[ps->i + 1] != '&')
		|| (input[ps->i + 1] == '&' && input[ps->i + 2] != '&'))
		return (error_handle_brackets(ERRNO_FOUR, ps->var));
	return (ps->i);
}

int	handle_operator(char *op, const char *input,
	t_status **st_head, t_parser *ps)
{
	ps->i = add_operator_node(op, &ps->cmds, st_head, ps);
	return (skip_spaces(input, ps->i));
}

t_status	*sep_input_to_cmds(const char *input, t_brackets *brackets,
	t_status *st_head, t_var **var)
{
	t_parser	ps;
	int		i;

	ps.i = -1;
	ps.cmds = NULL;
	ps.var = var;
	if (input[0] == '#')
		return (update_exit_code(0, var), NULL);
	if (input[0] == ';')
		return (ft_eprintf
			("minishell: syntax error near unexpected token `;'\n"),
			update_exit_code(2, var), NULL);
	while (input[++ps.i])
	{
		if (input[ps.i] == '\'' || input[ps.i] == '\"')
			ps.i = handle_quotes(input, &ps.cmds, ps.i, ps.var);
		else if (input[ps.i] == '#')
		{
			if (ps.cmds)
			{
				if (add_command_node(&ps.cmds, &st_head, var) == ERROR)
					return (free_lst_status(st_head, NULL), NULL);
			}
			break ;
		}
		else if (input[ps.i] == '(')
			ps.i = handle_brackets(input, brackets, &st_head, &ps);
		else if (input[ps.i] == '|' && input[ps.i + 1] == '|')
			ps.i = handle_operator("||", input, &st_head, &ps);
		else if (input[ps.i] == '&' && input[ps.i + 1] == '&')
			ps.i = handle_operator("&&", input, &st_head, &ps);
		else if (input[ps.i] == '|')
			ps.i = handle_operator("|", input, &st_head, &ps);
		else if (input[ps.i] == '&')
			ps.i = handle_operator("&", input, &st_head, &ps);
		else if (input[ps.i] == ';')
		{
			i = 1;
			while (input[ps.i + i] && input[ps.i + i] == ' ')
				i++;
			if (input[ps.i + i] == ';')
			{
				add_command_node(&ps.cmds, &st_head, var);
				free_lst_status(st_head, NULL);
				ft_eprintf
				("minishell: syntax error near unexpected token `;'\n");
				update_exit_code(2, var);
				return (NULL);
			}
			else
				ps.i = handle_operator(";", input, &st_head, &ps);
		}
		else
			ps.cmds = add_char(ps.cmds, input[ps.i], &ps.i, var);
		if (ps.i == ERROR)
			return (add_command_node(&ps.cmds, &st_head, var),
				free_lst_status(st_head, NULL), NULL);
	}
	if (add_command_node(&ps.cmds, &st_head, var) == ERROR)
		return (free_lst_status(st_head, NULL), NULL);
	return (st_head);
}
