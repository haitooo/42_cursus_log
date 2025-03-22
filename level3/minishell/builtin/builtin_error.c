/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 23:09:01 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/22 11:33:15 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	put_exporterr_msg(char *msg)
{
	int		total_len;
	char	*strerr;

	total_len = ft_strlen("minishell: export: `");
	total_len += ft_strlen("': not a valid identifier\n");
	total_len += ft_strlen(msg) + 2;
	strerr = (char *)malloc(sizeof(char) * total_len);
	if (!strerr)
	{
		free(msg);
		return ;
	}
	ft_strlcpy(strerr, "minishell: export: `", total_len);
	ft_strlcat(strerr, msg, total_len);
	ft_strlcat(strerr, "': not a valid identifier\n", total_len);
	ft_eprintf("%s", strerr);
	free(msg);
	free(strerr);
}

static void	put_redirecterr_msg(char *msg)
{
	int		total_len;
	char	*strerr;

	total_len = ft_strlen("minishell: ");
	total_len += ft_strlen("syntax error near unexpected token `");
	total_len += ft_strlen(msg) + 3;
	strerr = (char *)malloc(sizeof(char) * total_len);
	if (!strerr)
		return ;
	ft_strlcpy(strerr, "minishell: ", total_len);
	ft_strlcat(strerr, "syntax error near unexpected token `", total_len);
	ft_strlcat(strerr, msg, total_len);
	ft_strlcat(strerr, "'\n", total_len);
	ft_eprintf("%s", strerr);
	free(strerr);
}

static void	put_errmsg(char *msg1, char *msg2)
{
	int		total_len;
	char	*strerr;

	total_len = ft_strlen("minishell: ");
	total_len += ft_strlen(msg1) + ft_strlen(msg2) + 4;
	strerr = (char *)malloc(sizeof(char) * total_len);
	if (!strerr)
		return ;
	ft_strlcpy(strerr, "minishell: ", total_len);
	ft_strlcat(strerr, msg1, total_len);
	ft_strlcat(strerr, ": ", total_len);
	ft_strlcat(strerr, msg2, total_len);
	ft_strlcat(strerr, "\n", total_len);
	ft_eprintf("%s", strerr);
	free(strerr);
}

int	builtin_error(int errnum, char *msg)
{
	if (errnum == CMDERROR)
		put_errmsg(msg, "command not found");
	else if (errnum == REDIRECTERROR)
		put_redirecterr_msg(msg);
	else if (errnum == EXPORTERROR)
		put_exporterr_msg(msg);
	else if (errnum == AMBIGUOUS)
		put_errmsg(msg, "ambiguous redirect");
	else
		put_errmsg(msg, strerror(errnum));
	return (EXIT_FAILURE);
}
