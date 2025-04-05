/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sep_input_to_cmds.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 10:54:32 by haito             #+#    #+#             */
/*   Updated: 2025/04/04 01:25:50 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_operator(char *op, const char *input,
	t_status **st_head, t_parser *ps)
{
	ps->i = add_operator_node(op, &ps->cmds, st_head, ps);
	return (skip_spaces(input, ps->i));
}

int	check_first_char(char c, t_var **var)
{
	if (c == '#')
		return (update_exit_code(0, var), 1);
	if (c == ';')
		return (ft_eprintf
			("minishell: syntax error near unexpected token `;'\n"),
			update_exit_code(2, var), 1);
	return (0);
}

int	sep_brackets_and_operators(t_parser *ps,
	t_status **st_head, t_brackets *brackets, t_var **var)
{
	if (ps->input[ps->i] == '(')
		ps->i = handle_brackets(ps->input, brackets, st_head, ps);
	else if (ps->input[ps->i] == '|' && ps->input[ps->i + 1] == '|')
		ps->i = handle_operator("||", ps->input, st_head, ps);
	else if (ps->input[ps->i] == '&' && ps->input[ps->i + 1] == '&')
		ps->i = handle_operator("&&", ps->input, st_head, ps);
	else if (ps->input[ps->i] == '|')
		ps->i = handle_operator("|", ps->input, st_head, ps);
	else if (ps->input[ps->i] == '&')
		ps->i = handle_operator("&", ps->input, st_head, ps);
	else if (ps->input[ps->i] == ';')
	{
		ps->j = 1;
		while (ps->input[ps->i + ps->j] && ps->input[ps->i + ps->j] == ' ')
			ps->j++;
		if (ps->input[ps->i + ps->j] == ';')
			return (ft_eprintf
				("minishell: syntax error near unexpected token `;'\n"),
				add_command_node(&ps->cmds, st_head, var), ERROR);
		else
			ps->i = handle_operator(";", ps->input, st_head, ps);
	}
	else
		return (CONTINUE);
	return (SUCCESS);
}

int	sep_by_ope(t_parser *ps,
	t_status **st_head, t_brackets *brackets, t_var **var)
{
	int	result;

	result = 0;
	if (ps->input[ps->i] == '\'' || ps->input[ps->i] == '\"')
		ps->i = handle_quotes(ps->input, &ps->cmds, ps->i, ps->var);
	else if (ps->input[ps->i] == '#')
	{
		if (ps->cmds)
		{
			if (add_command_node(&ps->cmds, st_head, var) == ERROR)
				return (ERROR);
		}
		return (BREAK);
	}
	else
		result = sep_brackets_and_operators(ps, st_head, brackets, var);
	if (result == ERROR)
		return (update_exit_code(2, var), ERROR);
	if (result == CONTINUE)
		ps->cmds = add_char(ps->cmds, ps->input[ps->i], &ps->i, var);
	if (ps->i == ERROR)
		return (add_command_node(&ps->cmds, st_head, var), ERROR);
	return (0);
}

t_status	*sep_input_to_cmds(const char *input, t_brackets *brackets,
	t_status *st_head, t_var **var)
{
	t_parser	ps;
	int			result;

	init_ps(&ps, input, var);
	if (check_first_char(input[0], var))
		return (NULL);
	while (input[++ps.i])
	{
		result = sep_by_ope(&ps, &st_head, brackets, var);
		if (result == ERROR)
			return (free_lst_status(st_head, NULL), NULL);
		if (result == BREAK)
			break ;
	}
	if (add_command_node(&ps.cmds, &st_head, var) == ERROR)
		return (free_lst_status(st_head, NULL), NULL);
	return (st_head);
}
