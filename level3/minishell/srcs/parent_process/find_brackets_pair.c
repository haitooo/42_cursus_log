/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_brackets_pair.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 10:50:47 by haito             #+#    #+#             */
/*   Updated: 2025/03/22 07:45:21 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_brackets_pair(int i, t_brackets *brackets)
{
	int	n;

	n = 0;
	while (brackets->pair[n][0] != i)
		n++;
	return (brackets->pair[n][1]);
}

void	save_pair(t_brackets *b, int i)
{
	b->pair[b->countof_pair][0] = b->data[b->top--];
	b->pair[b->countof_pair++][1] = i;
	if (b->countof_pair < MAX_STACK_BRACKETS)
	{
		b->pair[b->countof_pair][0] = -1;
		b->pair[b->countof_pair][1] = -1;
	}
}

int	skip_quotes(const char *input, int i, char quote, t_var **var)
{
	while (input[i] && input[i] != quote)
		i++;
	if (!input[i])
	{
		update_exit_code(SYNERR, var);
		return (ft_eprintf("minishell: nothing quote pair %c\n", quote), -1);
	}
	return (i);
}

int	process_bracket(const char *input, t_brackets *b, int i)
{
	if (input[i] == '(')
	{
		if (b->top < MAX_STACK_BRACKETS - 1)
			b->data[++b->top] = i;
		else
		{
			ft_eprintf("minishell: error stack over flow\n");
			return (ERROR);
		}
	}
	else if (input[i] == ')')
	{
		if (b->top != -1)
			save_pair(b, i);
		else
		{
			ft_eprintf("minishell: syntax error near unexpected token `)'\n");
			return (ERROR);
		}
	}
	return (0);
}

int	find_brackets_pair(const char *input, t_brackets *b, int length,
		t_var **var)
{
	int	i;

	b->top = -1;
	b->countof_pair = 0;
	b->pair[0][0] = -1;
	b->pair[0][1] = -1;
	i = -1;
	while (++i < length)
	{
		if (input[i] == '\'' || input[i] == '"')
			i = skip_quotes(input, i + 1, input[i], var);
		else
		{
			if (process_bracket(input, b, i) == ERROR)
				return (update_exit_code(SYNERR, var), ERROR);
		}
		if (i == ERROR)
			return (ERROR);
	}
	if (b->top >= 0)
	{
		ft_eprintf("minishell: syntax error near unexpected token `('\n");
		return (update_exit_code(SYNERR, var), ERROR);
	}
	return (0);
}
