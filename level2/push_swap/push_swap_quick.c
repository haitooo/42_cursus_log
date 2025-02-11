#include "push_swap.h"

int	push_to_a(t_stacks *stacks, int push_index, int sizeof_args)
{
	int suit_index;

	suit_index = 0;
	while (suit_index < stacks->size_a)
	{
		if (suit_index == 0 && stacks->a[0].coord > stacks->b[push_index].coord
			&& (stacks->a[stacks->size_a - 1].coord < stacks->b[push_index].coord || stacks->a[stacks->size_a - 1].coord == sizeof_args - 1))
			break ;
		if (suit_index > 0 && (stacks->b[push_index].coord > stacks->a[suit_index - 1].coord  || stacks->a[suit_index - 1].coord == sizeof_args - 1)
			&& stacks->b[push_index].coord < stacks->a[suit_index].coord)
			break ;
		suit_index++;
	}
	if (stacks->size_b / 2 >= push_index && stacks->size_a / 2 >= suit_index)
		return (quick_a(stacks, push_index, suit_index));
	else if (stacks->size_b / 2 <= push_index && stacks->size_a / 2 <= suit_index)
		return (quick_b(stacks, push_index, suit_index));
	else if (stacks->size_b / 2 <= (push_index + 1) && stacks->size_a / 2 >= (suit_index + 1))
		return (quick_c(stacks, push_index, suit_index));
	else if (stacks->size_b / 2 >= (push_index + 1) && stacks->size_a / 2 <= (suit_index + 1))
		return (quick_d(stacks, push_index, suit_index));
	return (ft_printf("push_to_b Error\n"));
}

int	quick_calculate_steps(int index, t_stacks stacks, int sizeof_args)
{
	int	steps;
	int	suit_index;

	suit_index = 0;
	while (suit_index < stacks.size_a)
	{
		if (suit_index == 0 && stacks.a[0].coord > stacks.b[index].coord
			&& (stacks.a[stacks.size_a - 1].coord < stacks.b[index].coord || stacks.a[stacks.size_a - 1].coord == sizeof_args - 1))
			break ;
		if (suit_index > 0 && (stacks.b[index].coord > stacks.a[suit_index - 1].coord || stacks.a[suit_index - 1].coord == sizeof_args - 1)
			&& stacks.b[index].coord < stacks.a[suit_index].coord)
			break ;
		suit_index++;
	}
	if (stacks.size_b / 2 >= index && stacks.size_a / 2 >= suit_index)
		steps = which_bigger(index, suit_index);
	else if (stacks.size_b / 2 <= index && stacks.size_a / 2 <= suit_index)
		steps = which_bigger(stacks.size_b - index, stacks.size_a - suit_index);
	else if (stacks.size_b / 2 <= (index + 1) && stacks.size_a / 2 >= (suit_index + 1))
		steps = which_is_smallnum(suit_index + (stacks.size_b - index), which_bigger(index, suit_index), which_bigger(stacks.size_b - index, stacks.size_a - suit_index));
	else if (stacks.size_b / 2 >= (index + 1) && stacks.size_a / 2 <= (suit_index + 1))
		steps = which_is_smallnum(index + (stacks.size_a - suit_index), which_bigger(index, suit_index), which_bigger(stacks.size_b - index, stacks.size_a - suit_index));
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
		steps = quick_calculate_steps(index, stacks, sizeof_args);
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
	int count;

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

void	quick_sort(int sizeof_args, t_stacks stacks, int count)
{
	while (count <= 4)
	{
		while (stacks.size_b < (sizeof_args / 6) * (count + 2))
		{
			if (stacks.a[0].coord >= (sizeof_args / 6) * count && stacks.a[0].coord < (sizeof_args / 6) * (count + 1))
			{
				pb(&stacks, 0);
				rb(&stacks, 0, 0);
			}
			else if (stacks.a[0].coord >= (sizeof_args / 6) * (count + 1) && stacks.a[0].coord < (sizeof_args / 6) * (count + 2))
				pb(&stacks, 0);
			else
				ra(&stacks, 0, 0);
		}
		count += 2;
	}
	while (stacks.size_a > 7)
	{
		if (stacks.a[0].coord < sizeof_args - 7)
			pb(&stacks, 0);
		ra(&stacks, 0, 0);
	}
	sort_smallsize(&stacks);
	quick_sort_2(sizeof_args, stacks);
}
