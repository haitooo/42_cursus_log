/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_actions3_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 03:19:51 by haito             #+#    #+#             */
/*   Updated: 2025/02/13 04:59:22 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "checker_bonus.h"

void	ss(t_stacks *stacks, int non_print)
{
	sa(stacks, 1, 0);
	sb(stacks, 1, 0);
	if (!non_print)
		ft_printf("ss\n");
}

void	rr(t_stacks *stacks, int non_print)
{
	ra(stacks, 1, 0);
	rb(stacks, 1, 0);
	if (!non_print)
		ft_printf("rr\n");
}

void	rrr(t_stacks *stacks, int non_print)
{
	rra(stacks, 1, 0);
	rrb(stacks, 1, 0);
	if (!non_print)
		ft_printf("rrr\n");
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	n;

	n = 0;
	while (s1[n] && s2[n])
	{
		if (s1[n] != s2[n])
			return (s1[n] - s2[n]);
		n++;
	}
	return (s1[n] - s2[n]);
}
