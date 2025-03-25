/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_redirect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 23:08:52 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/24 11:50:49 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	redirect_out(t_tokens **tokens, t_type type, char *file)
{
	struct stat	path_stat;
	t_tokens	*head;
	int			filefd;

	head = *tokens;
	if (!ft_strcmp(head->next->token, "*"))
		return (builtin_error(AMBIGUOUS, head->next->token));
	else if (head->next->token[0] == '$' && head->next->type == VAR)
		return (builtin_error(AMBIGUOUS, head->next->token));
	if (!stat(head->next->token, &path_stat) && S_ISDIR(path_stat.st_mode))
		return (builtin_error(EISDIR, head->next->token));
	if (!access(file, F_OK) && access(file, W_OK))
		return (builtin_error(EACCES, file));
	if (type == TRUNC)
		filefd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		filefd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (filefd == -1)
		return (builtin_error(errno, file));
	if (dup2(filefd, STDOUT_FILENO) == -1)
		return (builtin_error(errno, file));
	if (close(filefd) == -1)
		return (builtin_error(errno, file));
	return (SUCCESS);
}

static int	redirect_in(t_tokens **tokens, t_type type, char *file)
{
	t_tokens	*head;
	t_tokens	*next;
	int			filefd;

	head = *tokens;
	next = head->next;
	if (type == INPUT && !ft_strcmp(next->token, "*"))
		return (builtin_error(AMBIGUOUS, head->next->token));
	if (type == INPUT && next->token[0] == '$' && next->type == VAR)
		return (builtin_error(AMBIGUOUS, head->next->token));
	if (access(file, F_OK))
		return (builtin_error(ENOENT, file));
	if (access(file, R_OK))
		return (builtin_error(EACCES, file));
	filefd = open(file, O_RDONLY);
	if (filefd == -1)
		return (builtin_error(errno, file));
	if (dup2(filefd, STDIN_FILENO) == -1)
		return (builtin_error(errno, file));
	if (close(filefd) == -1)
		return (builtin_error(errno, file));
	return (SUCCESS);
}

static int	redirects(t_tokens **tokens, char *tmpfile)
{
	t_tokens	*head;
	int			status;

	head = *tokens;
	status = SUCCESS;
	while (head)
	{
		if (head->type == HEREDOC)
			status = redirect_in(&head, head->type, tmpfile);
		else if (head->type == INPUT)
			status = redirect_in(&head, head->type, head->next->token);
		else if (head->type == TRUNC || head->type == APPEND)
			status = redirect_out(&head, head->type, head->next->token);
		if (status == EXIT_FAILURE)
			return (EXIT_FAILURE);
		head = head->next;
	}
	return (SUCCESS);
}

int	redirect_builtin(t_tokens **tokens, t_saved *saved, t_var **varlist)
{
	int		status;
	char	*tmpfile;

	tmpfile = NULL;
	status = check_builtin_redirect_syntax(tokens);
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
	delete_redirect(tokens);
	if (status != SUCCESS)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
