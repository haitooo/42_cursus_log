/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   make_envp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 22:00:48 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/18 18:46:25 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_vars(t_var **varlist)
{
	int		count;
	t_var	*head;

	count = 0;
	head = *varlist;
	while (head)
	{
		if (head->name && head->value)
			count++;
		head = head->next;
	}
	return (count);
}

static char	**concat_name_with_value(t_child *child, t_var **varlist, int size)
{
	t_var	*head;
	char	**envp;
	int		i;

	envp = (char **)malloc(sizeof(char *) * (size + 1));
	if (!envp)
		exit_child(child, EXIT_FAILURE, errno, "malloc error");
	i = 0;
	head = *varlist;
	while (head)
	{
		if (head->name && head->value)
		{
			envp[i] = ft_strjoin_three(head->name, "=", head->value);
			if (!envp[i])
				exit_child(child, EXIT_FAILURE, errno, "malloc error");
			i++;
		}
		head = head->next;
	}
	envp[i] = NULL;
	return (envp);
}

void	make_envp(t_child *child, t_var **varlist)
{
	int	malloc_size;

	if (!varlist || !*varlist)
		return ;
	malloc_size = count_vars(varlist);
	child->envp = concat_name_with_value(child, varlist, malloc_size);
}
