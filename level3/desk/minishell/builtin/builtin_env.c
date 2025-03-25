/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 23:09:01 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/22 17:09:52 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	env_syntax_error(char *token)
{
	char	*err;

	err = ft_strjoin_three("env: '", token, "’: No such file or directory\n");
	if (err)
		ft_eprintf("%s", err);
	free(err);
	return (EXIT_SYNTAX);
}

static int	put_errmsg(int errnum)
{
	int		total_len;
	char	*strerr;

	total_len = ft_strlen("minishell: malloc error: ");
	total_len += ft_strlen(strerror(errnum)) + 2;
	strerr = (char *)malloc(sizeof(char) * total_len);
	if (!strerr)
		return (EXIT_FAILURE);
	ft_strlcpy(strerr, "minishell: malloc error: ", total_len);
	ft_strlcat(strerr, strerror(errnum), total_len);
	ft_strlcat(strerr, "\n", total_len);
	ft_eprintf("%s", strerr);
	free(strerr);
	return (EXIT_FAILURE);
}

int	builtin_env(t_tokens **tokens, t_var **varlist)
{
	t_var	*head;
	int		total_len;
	char	*var;

	if ((*tokens)->next)
		return (env_syntax_error((*tokens)->next->token));
	head = *varlist;
	while (head)
	{
		if (head->value && ft_strncmp(head->name, "?", 2))
		{
			total_len = ft_strlen(head->name) + ft_strlen(head->value) + 3;
			var = (char *)malloc(sizeof(char) * total_len);
			if (!var)
				return (put_errmsg(errno));
			ft_strlcpy(var, head->name, total_len);
			ft_strlcat(var, "=", total_len);
			ft_strlcat(var, head->value, total_len);
			ft_strlcat(var, "\n", total_len);
			ft_putstr_fd(var, STDOUT_FILENO);
			free(var);
		}
		head = head->next;
	}
	return (EXIT_SUCCESS);
}
