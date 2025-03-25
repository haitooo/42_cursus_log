/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 22:01:01 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/23 15:06:53 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_dir(t_child *child, char *file)
{
	struct stat	path_stat;

	if (!stat(file, &path_stat) && S_ISDIR(path_stat.st_mode))
		exit_child(child, EXIT_FAILURE, EISDIR, file);
	return (SUCCESS);
}

static void	redirect(t_child *child, int oldfd, t_type type, char *file)
{
	int	filefd;

	if (type == TRUNC || type == APPEND)
	{
		if (is_dir(child, file) && !access(file, F_OK) && access(file, W_OK))
			exit_child(child, EXIT_FAILURE, EACCES, file);
		if (type == TRUNC)
			filefd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			filefd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	}
	else
	{
		if (access(file, F_OK))
			exit_child(child, EXIT_FAILURE, ENOENT, file);
		if (access(file, R_OK))
			exit_child(child, EXIT_FAILURE, EACCES, file);
		filefd = open(file, O_RDONLY);
	}
	if (filefd == -1)
		exit_child(child, EXIT_FAILURE, errno, file);
	if (dup2(filefd, oldfd) == -1)
		exit_child(child, EXIT_FAILURE, errno, file);
	if (close(filefd) == -1)
		exit_child(child, EXIT_FAILURE, errno, file);
}

void	redirect_fds(t_child *child, t_tokens **tokens, t_var **varlist)
{
	t_tokens	*head;
	t_tokens	*next;

	head = *tokens;
	while (head)
	{
		if (head->type == HEREDOC)
			child->tmpfile = child_heredoc(child, head->next->token,
					head->next->type, varlist);
		head = head->next;
	}
	head = *tokens;
	while (head)
	{
		next = head->next;
		if (head->type == HEREDOC)
			redirect(child, STDIN_FILENO, head->type, child->tmpfile);
		else if (next && next->token[0] == '$' && next->type == VAR)
			exit_child(child, EXIT_FAILURE, AMBIGUOUS, head->next->token);
		else if (next && !ft_strcmp(next->token, "*"))
			exit_child(child, EXIT_FAILURE, AMBIGUOUS, head->next->token);
		else if (head->type == INPUT)
			redirect(child, STDIN_FILENO, head->type, head->next->token);
		else if (head->type == TRUNC || head->type == APPEND)
			redirect(child, STDOUT_FILENO, head->type, head->next->token);
		head = head->next;
	}
}
