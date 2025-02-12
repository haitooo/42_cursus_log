/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap_turk_help.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 00:56:52 by haito             #+#    #+#             */
/*   Updated: 2025/02/13 02:35:15 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	pattern_max_num(t_stacks *stacks)
{
	int		suit_index_in_b;
	long	maxnum_stack_b;

	suit_index_in_b = 0;
	maxnum_stack_b = get_max_num(stacks->b, stacks->size_b);
	while (suit_index_in_b < stacks->size_b)
	{
		if (stacks->b[suit_index_in_b].num == maxnum_stack_b)
			return (suit_index_in_b);
		suit_index_in_b++;
	}
	return (suit_index_in_b);
}

int	pattern_min_num(t_stacks *stacks)
{
	int		suit_index_in_b;
	long	minnum_stack_b;

	suit_index_in_b = 0;
	minnum_stack_b = get_min_num(stacks->b, stacks->size_b);
	while (suit_index_in_b < stacks->size_b)
	{
		if (stacks->b[suit_index_in_b].num == minnum_stack_b)
			return (suit_index_in_b + 1);
		suit_index_in_b++;
	}
	return (suit_index_in_b);
}

int	maxnum_b_index(t_stacks stacks, long maxnum_stack_b)
{
	int	index;

	index = 0;
	while (stacks.b[index].num != maxnum_stack_b)
		index++;
	return (index);
}

int	choose_push_index(t_stacks stacks)
{
	int	index;
	int	best_index;
	int	min_steps;

	index = 0;
	best_index = -1;
	min_steps = -1;
	while (index < stacks.size_a)
	{
		calculate_steps(index, stacks, &min_steps, &best_index);
		index++;
	}
	if (best_index == -1 || min_steps == -1)
		return (ft_printf("Error\n", -1));
	return (best_index);
}
