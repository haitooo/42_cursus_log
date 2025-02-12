/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap_turk_help4.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 00:56:56 by haito             #+#    #+#             */
/*   Updated: 2025/02/13 01:14:02 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

void	sort_ele3(t_stacks *stacks)
{
	long	a;
	long	b;
	long	c;

	a = stacks->a[0].num;
	b = stacks->a[1].num;
	c = stacks->a[2].num;
	if (which_is_smallnum(a, b, c) == a && b < c)
		return ;
	if (which_is_smallnum(a, b, c) == a && b > c)
	{
		rra(stacks, 0, 0);
		sa(stacks, 0, 0);
	}
	if (which_is_smallnum(a, b, c) == b && a < c)
		sa(stacks, 0, 0);
	if (which_is_smallnum(a, b, c) == b && a > c)
		ra(stacks, 0, 0);
	if (which_is_smallnum(a, b, c) == c && a < b)
		rra(stacks, 0, 0);
	if (which_is_smallnum(a, b, c) == c && a > b)
	{
		ra(stacks, 0, 0);
		sa(stacks, 0, 0);
	}
}

void	sort_ele4(t_stacks *stacks)
{
	int		min_index;
	long	min_num_a;

	min_index = -1;
	min_num_a = 9000000000;
	get_min(stacks, &min_num_a, &min_index);
	if (min_index == 1 && stacks->a[2].num
		< stacks->a[1].num && stacks->a[2].num < stacks->a[3].num)
	{
		sa(stacks, 0, 0);
		return ;
	}
	if (min_index == 1 || min_index == 2)
		ra(stacks, 0, 0);
	if (min_index == 2)
		ra(stacks, 0, 0);
	if (min_index == 3)
		rra(stacks, 0, 0);
	pb(stacks, 0);
	sort_ele3(stacks);
	pa(stacks, 0);
}

void	sort_ele5(t_stacks *stacks)
{
	int		min_index;
	long	min_num_a;

	min_index = -1;
	min_num_a = 9000000000;
	get_min(stacks, &min_num_a, &min_index);
	if (min_index == 1 || min_index == 2)
		ra(stacks, 0, 0);
	if (min_index == 2)
		ra(stacks, 0, 0);
	if (min_index == 3)
		rra(stacks, 0, 0);
	if (min_index == 3 || min_index == 4)
		rra(stacks, 0, 0);
	pb(stacks, 0);
	sort_ele4(stacks);
	pa(stacks, 0);
}

void	sort_ele6(t_stacks *stacks)
{
	int		min_index;
	long	min_num_a;

	min_index = -1;
	min_num_a = 9000000000;
	get_min(stacks, &min_num_a, &min_index);
	if (min_index == 1 || min_index == 2 || min_index == 3)
		ra(stacks, 0, 0);
	if (min_index == 2 || min_index == 3)
		ra(stacks, 0, 0);
	if (min_index == 3)
		ra(stacks, 0, 0);
	if (min_index == 4)
		rra(stacks, 0, 0);
	if (min_index == 4 || min_index == 5)
		rra(stacks, 0, 0);
	pb(stacks, 0);
	sort_ele5(stacks);
	pa(stacks, 0);
}

void	sort_ele7(t_stacks *stacks)
{
	int		min_index;
	long	min_num_a;

	min_index = -1;
	min_num_a = 9000000000;
	get_min(stacks, &min_num_a, &min_index);
	if (min_index == 1 || min_index == 2 || min_index == 3)
		ra(stacks, 0, 0);
	if (min_index == 2 || min_index == 3)
		ra(stacks, 0, 0);
	if (min_index == 3)
		ra(stacks, 0, 0);
	if (min_index == 4)
		rra(stacks, 0, 0);
	if (min_index == 4 || min_index == 5)
		rra(stacks, 0, 0);
	if (min_index == 4 || min_index == 5 || min_index == 6)
		rra(stacks, 0, 0);
	pb(stacks, 0);
	sort_ele6(stacks);
	pa(stacks, 0);
}
