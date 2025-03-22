/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_redirect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 23:08:52 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/20 19:28:03 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_syntax(t_tokens **tokens)
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

static int	redirect(int oldfd, t_type type, char *file, int filefd)
{
	if (type == TRUNC || type == APPEND)
	{
		if (!access(file, F_OK) && access(file, W_OK))
			return (builtin_error(EACCES, file));
		if (type == TRUNC)
			filefd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			filefd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	else
	{
		if (access(file, F_OK))
			return (builtin_error(ENOENT, file));
		if (access(file, R_OK))
			return (builtin_error(EACCES, file));
		filefd = open(file, O_RDONLY);
	}
	if (filefd == -1)
		return (builtin_error(errno, file));
	if (dup2(filefd, oldfd) == -1)
		return (builtin_error(errno, file));
	if (close(filefd) == -1)
		return (builtin_error(errno, file));
	return (SUCCESS);
}

static int	redirects(t_tokens **tokens, char *tmpfile)
{
	t_tokens	*head;
	t_tokens	*next;
	struct stat	path_stat;
	int			status;

	head = *tokens;
	status = SUCCESS;
	while (head)
	{
		next = head->next;
		if (head->type == HEREDOC)
			status = redirect(STDIN_FILENO, head->type, tmpfile, -1);
		else if (next && next->token[0] == '$' && next->type == VAR)
			return (builtin_error(AMBIGUOUS, head->next->token));
		else if (head->type == INPUT)
			status = redirect(STDIN_FILENO, head->type, head->next->token, -1);
		else if (head->type == TRUNC || head->type == APPEND)
		{
			if (!stat(head->next->token, &path_stat) && S_ISDIR(path_stat.st_mode))
				return (builtin_error(EISDIR, head->next->token));
			status = redirect(STDOUT_FILENO, head->type, head->next->token, -1);
		}
		if (status != SUCCESS)
			return (EXIT_FAILURE);
		head = head->next;
	}
	return (SUCCESS);
}

static int	delete_redirect(t_tokens **tokens)
{
	t_tokens	*head;
	t_tokens	*tmp;

	head = *tokens;
	while (head)
	{
		if (head->type != WORD && head->type != HAVE_QUOTE)
		{
			tmp = head;
			head = head->next;
			free_one_token(tokens, tmp);
			tmp = head;
			head = head->next;
			free_one_token(tokens, tmp);
		}
		else
			head = head->next;
	}
	return (SUCCESS);
}

int	redirect_builtin(t_tokens **tokens, t_saved *saved, t_var **varlist)
{
	int		status;
	char	*tmpfile;

	tmpfile = NULL;
	status = check_syntax(tokens);
	if (status != SUCCESS)
		return (EXIT_FAILURE);
	status = builtin_save_stdio(tokens, saved);
	if (status != SUCCESS)
		return (EXIT_FAILURE);
	status = check_here_doc(tokens, &tmpfile, varlist);
	if (status != SUCCESS)
		return (EXIT_FAILURE);
	status = redirects(tokens, tmpfile);
	if (tmpfile)
	{
		unlink(tmpfile);
		free(tmpfile);
	}
	if (status != SUCCESS)
		return (EXIT_FAILURE);
	delete_redirect(tokens);
	return (EXIT_SUCCESS);
}
