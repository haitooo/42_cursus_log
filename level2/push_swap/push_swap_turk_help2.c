/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap_turk_help2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 00:56:54 by haito             #+#    #+#             */
/*   Updated: 2025/02/13 02:09:29 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	pattern_a(t_stacks *stacks, int push_index,
	int suit_index_in_b, int non_print)
{
	int		count;

	count = 0;
	while (count < push_index && count < suit_index_in_b
		&& push_index != 0 && suit_index_in_b != 0)
	{
		rr(stacks, non_print);
		count++;
	}
	while (count < push_index || count < suit_index_in_b)
	{
		if (count < push_index && push_index != 0)
			ra(stacks, 0, non_print);
		if (count < suit_index_in_b && suit_index_in_b != 0)
			rb(stacks, 0, non_print);
		count++;
	}
	return (pb(stacks, non_print), 0);
}

int	pattern_b(t_stacks *stacks, int push_index,
	int suit_index_in_b, int non_print)
{
	int		count;

	count = 0;
	while (count < stacks->size_a - push_index
		&& count < stacks->size_b - suit_index_in_b
		&& push_index != stacks->size_a && suit_index_in_b != stacks->size_b)
	{
		rrr(stacks, non_print);
		count++;
	}
	while (count < stacks->size_a - push_index
		|| count < stacks->size_b - suit_index_in_b)
	{
		if (count < stacks->size_a - push_index
			&& push_index != stacks->size_a && push_index != 0)
			rra(stacks, 0, non_print);
		if (count < stacks->size_b - suit_index_in_b
			&& suit_index_in_b != stacks->size_b)
			rrb(stacks, 0, non_print);
		count++;
	}
	return (pb(stacks, non_print), 0);
}

int	pattern_c(t_stacks *stacks, int push_index, int suit_b, int non_print)
{
	int		count;

	count = 0;
	if (push_index + (stacks->size_b - suit_b) <= bigger(push_index,
			suit_b) && push_index + (stacks->size_b - suit_b)
		<= bigger(stacks->size_a - push_index, stacks->size_b - suit_b))
	{
		while (count < push_index)
		{
			ra(stacks, 0, non_print);
			count++;
		}
		count = 0;
		while (count < stacks->size_b - suit_b)
		{
			rrb(stacks, 0, non_print);
			count++;
		}
		return (pb(stacks, non_print), 0);
	}
	if (bigger(push_index, suit_b)
		< bigger(stacks->size_a - push_index, stacks->size_b - suit_b))
		return (pattern_a(stacks, push_index, suit_b, non_print));
	else
		return (pattern_b(stacks, push_index, suit_b, non_print));
}

int	pattern_d(t_stacks *stacks, int push_index, int suit_b, int non_print)
{
	int		count;

	count = 0;
	if (suit_b + (stacks->size_a - push_index) <= bigger(push_index,
			suit_b) && suit_b + (stacks->size_a - push_index)
		<= bigger(stacks->size_a - push_index, stacks->size_b - suit_b))
	{
		while (count < (stacks->size_a - push_index))
		{
			rra(stacks, 0, non_print);
			count++;
		}
		count = 0;
		while (count < suit_b)
		{
			rb(stacks, 0, non_print);
			count++;
		}
		return (pb(stacks, non_print), 0);
	}
	if (bigger(push_index, suit_b)
		< bigger(stacks->size_a - push_index, stacks->size_b - suit_b))
		return (pattern_a(stacks, push_index, suit_b, non_print));
	else
		return (pattern_b(stacks, push_index, suit_b, non_print));
}

void	get_min(t_stacks *stacks, long *min_num_a, int *min_index)
{
	int	count;

	count = 0;
	while (count < stacks->size_a)
	{
		if (stacks->a[count].num < *min_num_a)
		{
			*min_num_a = stacks->a[count].num;
			*min_index = count;
		}
		count++;
	}
}
