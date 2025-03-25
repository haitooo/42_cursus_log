/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 14:21:12 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/23 23:36:52 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_tokens	*remove_empty_top_tokens(t_tokens **tokens)
{
	t_tokens	*tmp;

	if (*tokens && !(*tokens)->token[0])
	{
		tmp = *tokens;
		*tokens = (*tokens)->next;
		free(tmp->token);
		free(tmp);
	}
	return (*tokens);
}

static t_tokens	*remove_empty_tokens(t_tokens **tokens)
{
	t_tokens	*head;
	t_tokens	*prev;
	t_tokens	*tmp;

	prev = NULL;
	head = *tokens;
	while (head)
	{
		if (!head->token || !head->token[0])
		{
			tmp = head;
			prev->next = head->next;
			head = head->next;
			free(tmp->token);
			free(tmp);
		}
		else
		{
			prev = head;
			head = head->next;
		}
	}
	return (*tokens);
}

static t_type	check_first_token(char *str)
{
	int	i;

	i = 0;
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '$')
		return (VAR);
	return (WORD);
}

static int	replace_home(t_tokens **tokens, t_var **varlist)
{
	t_tokens	*head;
	t_var		*home;
	char		*new_token;
	int			size;

	home = get_var(varlist, "HOME");
	if (!home || !home->value || !home->value[0])
		return (SUCCESS);
	head = *tokens;
	while (head)
	{
		if (!ft_strncmp(head->token, "~/", 2))
		{
			size = ft_strlen(head->token) + ft_strlen(home->value);
			new_token = (char *)malloc(sizeof(char) * size);
			if (!new_token)
				return (ERROR);
			ft_strlcpy(new_token, home->value, size + 1);
			ft_strlcat(new_token, &(head->token[1]), size + 1);
			free(head->token);
			head->token = new_token;
		}
		head = head->next;
	}
	return (SUCCESS);
}

t_tokens	*expander(char *str, t_var **varlist)
{
	t_tokens	*tokens;
	t_type		check;

	tokens = NULL;
	tokens = pre_tokenizer(str);
	if (!tokens)
		return (NULL);
	check = check_first_token(tokens->token);
	if (replace_vars(&tokens, varlist) == ERROR)
		return (free_tokens(&tokens), NULL);
	tokens = remove_empty_top_tokens(&tokens);
	tokens = remove_empty_tokens(&tokens);
	expand_wildcard(&tokens);
	more_tokenizer(&tokens, check);
	replace_home(&tokens, varlist);
	return (tokens);
}
