/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_vars.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/29 13:43:19 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/30 15:07:32 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	replace_vars(t_splited **splited, t_var **varlist)
{
	t_splited	*head;
	t_var		*var;

	head = *splited;
	while (head)
	{
		if (head->type == PLAINVAR || head->type == QUOTEVAR)
		{
			var = get_var(varlist, &head->str[1]);
			free(head->str);
			if (!var || !var->value)
				head->str = ft_strdup("");
			else
				head->str = ft_strdup(var->value);
			if (!head->str)
				return ;
		}
		head = head->next;
	}
}
