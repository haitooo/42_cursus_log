/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap_actions2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 00:49:12 by haito             #+#    #+#             */
/*   Updated: 2025/02/13 20:06:24 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	rra(t_stacks *stacks, int is_rrr, int non_print)
{
	t_date	tmp;
	int		i;

	i = 2;
	tmp = (stacks->a)[stacks->size_a - 1];
	while (stacks->size_a - i >= 0)
	{
		(stacks->a)[stacks->size_a - (i - 1)] = (stacks->a)[stacks->size_a - i];
		i++;
	}
	(stacks->a)[0] = tmp;
	if (!is_rrr && !non_print)
		ft_printf("rra\n");
}

void	rrb(t_stacks *stacks, int is_rrr, int non_print)
{
	t_date	tmp;
	int		i;

	i = 2;
	tmp = (stacks->b)[stacks->size_b - 1];
	while (stacks->size_b - i >= 0)
	{
		(stacks->b)[stacks->size_b - (i - 1)] = (stacks->b)[stacks->size_b - i];
		i++;
	}
	(stacks->b)[0] = tmp;
	if (!is_rrr && !non_print)
		ft_printf("rrb\n");
}

void	pa(t_stacks *stacks, int non_print)
{
	int	i;

	i = 0;
	if (stacks->size_b <= 0)
		return ;
	while (i < stacks->size_a)
	{
		(stacks->a)[stacks->size_a - i] = (stacks->a)[stacks->size_a - 1 - i];
		i++;
	}
	(stacks->a)[0] = (stacks->b)[0];
	i = 0;
	while (i < (stacks->size_b - 1))
	{
		(stacks->b)[i] = (stacks->b)[i + 1];
		i++;
	}
	(stacks->b)[i].num = EMPTY_INDEX_NUM;
	(stacks->b)[i].coord = -1;
	stacks->size_a += 1;
	stacks->size_b -= 1;
	if (!non_print)
		ft_printf("pa\n");
}

void	pb(t_stacks *stacks, int non_print)
{
	int	i;

	i = 0;
	if (stacks->size_a <= 0)
		return ;
	while (i < stacks->size_b)
	{
		(stacks->b)[stacks->size_b - i] = (stacks->b)[stacks->size_b - 1 - i];
		i++;
	}
	(stacks->b)[0] = (stacks->a)[0];
	i = 0;
	while (i < (stacks->size_a - 1))
	{
		(stacks->a)[i] = (stacks->a)[i + 1];
		i++;
	}
	(stacks->a)[i].num = EMPTY_INDEX_NUM;
	(stacks->a)[i].coord = -1;
	stacks->size_b += 1;
	stacks->size_a -= 1;
	if (!non_print)
		ft_printf("pb\n");
}
