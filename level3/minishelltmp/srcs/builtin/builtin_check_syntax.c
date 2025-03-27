/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_check_syntax.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 11:47:42 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/24 11:49:59 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_builtin_redirect_syntax(t_tokens **tokens)
{
	t_tokens	*head;
	t_type		type;

	head = *tokens;
	while (head)
	{
		type = head->type;
		if (type != WORD && type != HAVE_QUOTE && type != VAR)
		{
			if (!head->next)
				return (builtin_error(REDIRECTERROR, "newline"));
			type = head->next->type;
			if (type != WORD && type != HAVE_QUOTE && type != VAR)
				return (builtin_error(REDIRECTERROR, head->next->token));
			head = head->next->next;
		}
		else
			head = head->next;
	}
	return (EXIT_SUCCESS);
}
