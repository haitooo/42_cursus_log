/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap_turk_help3.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 00:56:55 by haito             #+#    #+#             */
/*   Updated: 2025/02/13 01:20:54 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	case_sorted_a(t_stacks *stacks)
{
	int		i;
	long	min_num_stack_a;

	i = 0;
	min_num_stack_a = get_min_num(stacks->a, stacks->size_a);
	while (min_num_stack_a != stacks->a[i].num)
		i++;
	if (i <= stacks->size_a / 2)
		while (stacks->a[0].num != min_num_stack_a)
			ra(stacks, 0, 0);
	else
		while (stacks->a[0].num != min_num_stack_a)
			rra(stacks, 0, 0);
}

int	get_suit_index(t_stacks stacks, int *best_index, int *min_steps, int index)
{
	int		suit_index_in_b;
	long	maxnum_stack_b;

	suit_index_in_b = 0;
	maxnum_stack_b = get_max_num(stacks.b, stacks.size_b);
	while (suit_index_in_b < stacks.size_b)
	{
		if (suit_index_in_b == 0 && stacks.b[0].num == maxnum_stack_b
			&& stacks.a[index].num > maxnum_stack_b)
		{
			*best_index = 0;
			*min_steps = 0;
			return (0);
		}
		if (suit_index_in_b == 0 && stacks.a[index].num > stacks.b[0].num
			&& stacks.a[index].num < stacks.b[stacks.size_b - 1].num)
			break ;
		if (suit_index_in_b > 0 && stacks.a[index].num
			< stacks.b[suit_index_in_b - 1].num && stacks.a[index].num
			> stacks.b[suit_index_in_b].num)
			break ;
		suit_index_in_b++;
	}
	return (suit_index_in_b);
}

int	check_sorted_a(t_stacks stacks, int sizeof_args,
	int i, int count)
{
	long	min_num;

	min_num = get_min_num(stacks.a, stacks.size_a);
	while (stacks.a[0].num != min_num)
	{
		ra(&stacks, 0, 1);
		count++;
	}
	while (++i < (sizeof_args - 1))
	{
		if (stacks.a[i].num > stacks.a[i + 1].num)
		{
			while (count > 0)
			{
				rra(&stacks, 0, 1);
				count--;
			}
			return (0);
		}
	}
	count++;
	while (--count > 0)
		rra(&stacks, 0, 1);
	return (1);
}

int	check_sorted(t_stacks stacks, int sizeof_args)
{
	int	is_sorted;
	int	is_not_sorted;
	int	i;

	is_sorted = 1;
	is_not_sorted = 0;
	i = 0;
	if (sizeof_args != stacks.size_a)
		return (is_not_sorted);
	while (i < (sizeof_args - 1))
	{
		if (stacks.a[i].num > stacks.a[i + 1].num)
			return (is_not_sorted);
		i++;
	}
	return (is_sorted);
}
