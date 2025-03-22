/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 23:08:58 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/22 11:32:09 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *make_error_word(char *str)
{
	int i;

	i = 0;
	while(str[i] && !ft_isspace(str[i]))
			i++;
	return ft_strndup(str, i);
}

static int	check_syntax(char *token)
{
	int		i;
	int		flag;
	char	*var;

	flag = 0;
	if (ft_isdigit(token[0]))
		flag = 1;
	i = 0;
	while (token[i] && token[i] != '=')
	{
		if (!ft_strncmp(&token[i], "+=", 2))
			break ;
		if (!ft_isalnum(token[i]) && token[i] != '_')
			flag = 1;
		i++;
	}
	if (i && !flag)
		return (SUCCESS);
	if(!i)
		var = make_error_word(token);
	else
		var = ft_strndup(token, i);
	if (!var)
		return (builtin_error(errno, "malloc error"));
	return (builtin_error(EXPORTERROR, var));
}

static int	append_var(t_var **varlist, char *str, int name_len)
{
	char	*name;
	char	*value;
	char	*base_value;
	t_var	*base;

	name = ft_strndup(str, name_len);
	if (!name)
		return (builtin_error(errno, "malloc error"));
	value = ft_strdup(&str[name_len + 2]);
	if (!value)
		return (free(name), builtin_error(errno, "malloc error"));
	base = get_var(varlist, name);
	if (!base)
		add_var(varlist, name, value);
	else
	{
		free(name);
		base_value = base->value;
		base->value = ft_strjoin(base_value, value);
		free(base_value);
		free(value);
		if (!base->value)
			return (builtin_error(errno, "malloc error"));
	}
	return (SUCCESS);
}

static int	add_newvar(t_var **varlist, char *str, int name_len)
{
	char	*name;
	char	*value;

	name = ft_strndup(str, name_len);
	if (!name)
		return (builtin_error(errno, "malloc error"));
	if (!str[name_len])
		value = NULL;
	else
	{
		value = ft_strdup(&str[name_len + 1]);
		if (!value)
		{
			free(name);
			return (builtin_error(errno, "malloc error"));
		}
	}
	add_var(varlist, name, value);
	return (SUCCESS);
}

int	builtin_export(t_tokens **tokens, t_var **varlist, int exit_num)
{
	t_tokens	*head;
	int			i;

	head = (*tokens)->next;
	if (!head)
		return (builtin_export_list(varlist));
	while (head)
	{
		if (check_syntax(head->token) == SUCCESS)
		{
			i = 0;
			while (head->token[i] && head->token[i] != '='
				&& head->token[i] != '+')
				i++;
			if (head->token[i] == '+')
				append_var(varlist, head->token, i);
			else
				add_newvar(varlist, head->token, i);
		}
		else
			exit_num = 1;
		head = head->next;
	}
	return (exit_num);
}
