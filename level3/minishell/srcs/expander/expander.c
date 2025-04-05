/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 14:21:12 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/30 13:18:20 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_tokens	*expander(char *str, t_var **varlist)
{
	t_tokens	*tokens;

	tokens = NULL;
	tokens = tokenizer(str);
	if (!tokens)
		return (NULL);
	tokens = expand_tokens(&tokens, varlist);
	tokens = remove_empty_tokens(&tokens);
	if (tokens)
		replace_home(&tokens, varlist);
	return (tokens);
}
