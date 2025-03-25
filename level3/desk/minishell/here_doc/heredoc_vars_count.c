/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_vars_count.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 12:53:16 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/18 22:06:41 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*count_envvar_size(char *str)
{
	if (*str == '$')
		str++;
	while (*str && (ft_isalnum(*str) || *str == '_'))
		str++;
	return (str);
}

static char	*heredoc_count_plaintext_size(char *str)
{
	while (*str == '$')
		str++;
	while (*str && *str != '$')
		str++;
	return (str);
}

int	heredoc_vars_count(char *str)
{
	int	malloc_size;

	malloc_size = 0;
	while (*str)
	{
		malloc_size++;
		if (*str == '$' && (ft_isalnum(*(str + 1)) || *(str + 1) == '_'))
			str = count_envvar_size(str);
		else
			str = heredoc_count_plaintext_size(str);
	}
	return (malloc_size);
}
