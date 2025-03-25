/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   continue_child.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 21:52:57 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/24 11:50:31 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	check_child_syntax(t_child *child, t_tokens **tokens)
{
	t_tokens	*head;
	t_type		type;

	head = *tokens;
	while (head)
	{
		type = head->type;
		if (type != WORD && type != HAVE_QUOTE && type != VAR)
		{
			head = head->next;
			if (!head)
				exit_child(child, EXIT_SYNTAX, REDIRECTERROR, "newline");
			type = head->type;
			if (type != WORD && type != HAVE_QUOTE && type != VAR)
				exit_child(child, EXIT_SYNTAX, REDIRECTERROR, head->token);
			head = head->next;
		}
		else
			head = head->next;
	}
}

void	continue_child(t_tokens **tokens, t_var **varlist)
{
	t_child	child;

	child.cmds = NULL;
	child.envp = NULL;
	child.paths = NULL;
	child.fullpath = NULL;
	child.tmpfile = NULL;
	child.tokens = tokens;
	child.varlist = varlist;
	check_child_syntax(&child, child.tokens);
	call_heredoc(&child, tokens, varlist);
	redirect_fds(&child, child.tokens);
	make_cmds(&child, child.tokens);
	make_envp(&child, child.varlist);
	make_fullpath(&child, child.cmds[0], child.varlist);
	execve(child.fullpath, child.cmds, child.envp);
	exit_child(&child, EXIT_FAILURE, errno, child.cmds[0]);
}
