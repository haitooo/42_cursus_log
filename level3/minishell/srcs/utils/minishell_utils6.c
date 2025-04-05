/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils6.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 17:50:55 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/31 05:55:34 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isupper(int c)
{
	return ('A' <= c && c <= 'Z');
}

int	ft_islower(int c)
{
	return ('a' <= c && c <= 'z');
}

int	ft_isalpha(int c)
{
	return (ft_islower(c) || ft_isupper(c));
}

char	*get_path(t_var **varlist, char *path)
{
	t_var	*var;

	var = *varlist;
	while (var && ft_strcmp(var->name, path) != 0)
		var = var->next;
	if (!var)
		return (NULL);
	return (var->value);
}
