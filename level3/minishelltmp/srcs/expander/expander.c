/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 14:21:12 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/26 20:30:02 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_type	check_first_token_type(char *str)
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
	t_type		type;

	tokens = NULL;
	tokens = tokenizer(str);
	if (!tokens)
		return (NULL);
	type = check_first_token_type(tokens->token);
	if (replace_vars(&tokens, varlist) == ERROR)
		return (free_tokens(&tokens), NULL);
	tokens = remove_empty_tokens(&tokens);
	expand_wildcard(&tokens);
	remove_quotes_and_split_tokens(&tokens, type);
	replace_home(&tokens, varlist);
	return (tokens);
}
