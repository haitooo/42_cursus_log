/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap_quick.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 00:56:50 by haito             #+#    #+#             */
/*   Updated: 2025/02/13 02:29:52 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	push_to_a(t_stacks *s, int p_idx, int size_ag)
{
	int	suit;

	suit = 0;
	while (suit < s->size_a)
	{
		if (suit == 0 && s->a[0].coord > s->b[p_idx].coord
			&& (s->a[s->size_a - 1].coord < s->b[p_idx].coord
				|| s->a[s->size_a - 1].coord == size_ag - 1))
			break ;
		if (suit > 0 && (s->b[p_idx].coord > s->a[suit - 1].coord
				|| s->a[suit - 1].coord == size_ag - 1)
			&& s->b[p_idx].coord < s->a[suit].coord)
			break ;
		suit++;
	}
	if (s->size_b / 2 >= p_idx && s->size_a / 2 >= suit)
		return (quick_a(s, p_idx, suit));
	else if (s->size_b / 2 <= p_idx && s->size_a / 2 <= suit)
		return (quick_b(s, p_idx, suit));
	else if (s->size_b / 2 <= (p_idx + 1) && s->size_a / 2 >= (suit + 1))
		return (quick_c(s, p_idx, suit));
	else if (s->size_b / 2 >= (p_idx + 1) && s->size_a / 2 <= (suit + 1))
		return (quick_d(s, p_idx, suit));
	return (ft_printf("push_to_b Error\n"));
}

int	quick_calculate_steps(int idx, t_stacks s, int size_ag, int steps)
{
	int	suit;

	suit = -1;
	while (++suit < s.size_a)
	{
		if (suit == 0 && s.a[0].coord > s.b[idx].coord
			&& (s.a[s.size_a - 1].coord < s.b[idx].coord
				|| s.a[s.size_a - 1].coord == size_ag - 1))
			break ;
		if (suit > 0 && (s.b[idx].coord > s.a[suit - 1].coord
				|| s.a[suit - 1].coord == size_ag - 1)
			&& s.b[idx].coord < s.a[suit].coord)
			break ;
	}
	if (s.size_b / 2 >= idx && s.size_a / 2 >= suit)
		steps = bigger(idx, suit);
	else if (s.size_b / 2 <= idx && s.size_a / 2 <= suit)
		steps = bigger(s.size_b - idx, s.size_a - suit);
	else if (s.size_b / 2 <= (idx + 1) && s.size_a / 2 >= (suit + 1))
		steps = which_is_smallnum(suit + s.size_b - idx, bigger(idx, suit),
				bigger(s.size_b - idx, s.size_a - suit));
	else if (s.size_b / 2 >= (idx + 1) && s.size_a / 2 <= (suit + 1))
		steps = which_is_smallnum(idx + s.size_a - suit, bigger(idx, suit),
				bigger(s.size_b - idx, s.size_a - suit));
	return (steps);
}

int	quick_choose_index(t_stacks stacks, int sizeof_args)
{
	int	index;
	int	best_index;
	int	min_steps;
	int	steps;

	index = 0;
	best_index = -1;
	min_steps = -1;
	while (index < stacks.size_b)
	{
		steps = quick_calculate_steps(index, stacks, sizeof_args, 0);
		if (min_steps > steps || min_steps == -1)
		{
			min_steps = steps;
			best_index = index;
		}
		index++;
	}
	if (best_index == -1 || min_steps == -1)
		return (ft_printf("choose_index Error\n", -1));
	return (best_index);
}

void	quick_sort_2(int sizeof_args, t_stacks stacks)
{
	int	push_index;
	int	count;

	if (sizeof_args % 6 == 0)
		case_zero(&stacks, sizeof_args);
	while (stacks.size_b > 0)
	{
		push_index = quick_choose_index(stacks, sizeof_args);
		push_to_a(&stacks, push_index, sizeof_args);
	}
	count = 0;
	while (stacks.a[count].coord != 0)
			count++;
	if (count <= sizeof_args / 2)
		while (stacks.a[0].coord != 0)
			ra(&stacks, 0, 0);
	else
		while (stacks.a[0].coord != 0)
			rra(&stacks, 0, 0);
}

void	quick_sort(int size_ag, t_stacks s, int n)
{
	while (n <= 4)
	{
		while (s.size_b < (size_ag / 6) * (n + 2))
		{
			if (s.a[0].coord >= (size_ag / 6) * n
				&& s.a[0].coord < (size_ag / 6) * (n + 1))
			{
				pb(&s, 0);
				rb(&s, 0, 0);
			}
			else if (s.a[0].coord >= (size_ag / 6) * (n + 1)
				&& s.a[0].coord < (size_ag / 6) * (n + 2))
				pb(&s, 0);
			else
				ra(&s, 0, 0);
		}
		n += 2;
	}
	sort_smallsize(&s);
	quick_sort_2(size_ag, s);
}
