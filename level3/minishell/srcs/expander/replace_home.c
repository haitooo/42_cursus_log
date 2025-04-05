/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_home.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 14:21:12 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/30 13:09:01 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	replace_home(t_tokens **tokens, t_var **varlist)
{
	t_tokens	*head;
	t_var		*home;
	char		*new_token;
	int			size;

	home = get_var(varlist, "HOME");
	if (!home || !home->value || !home->value[0])
		return (SUCCESS);
	head = *tokens;
	while (head)
	{
		if (!ft_strncmp(head->token, "~/", 2))
		{
			size = ft_strlen(head->token) + ft_strlen(home->value);
			new_token = (char *)malloc(sizeof(char) * size);
			if (!new_token)
				return (ERROR);
			ft_strlcpy(new_token, home->value, size + 1);
			ft_strlcat(new_token, &(head->token[1]), size + 1);
			free(head->token);
			head->token = new_token;
		}
		head = head->next;
	}
	return (SUCCESS);
}
