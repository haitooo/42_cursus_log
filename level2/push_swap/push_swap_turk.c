#include "push_swap.h"

int	push_to_b(t_stacks *stacks, int push_index, long *maxnum_stack_b, long *minnum_stack_b, int suit_index_in_b)
{
	while (suit_index_in_b < stacks->size_b)
	{
		if (suit_index_in_b > 0 && stacks->a[push_index].num < stacks->b[suit_index_in_b - 1].num
			&& stacks->a[push_index].num > stacks->b[suit_index_in_b].num)
			break ;
		suit_index_in_b++;
	}
	if (stacks->a[push_index].num > *maxnum_stack_b)
	{
		suit_index_in_b = pattern_max_num(stacks, maxnum_stack_b);
		*maxnum_stack_b = stacks->a[push_index].num;
	}
	if (stacks->a[push_index].num < *minnum_stack_b)
	{
		suit_index_in_b = pattern_min_num(stacks, minnum_stack_b);
		*minnum_stack_b = stacks->a[push_index].num;
	}
	if (stacks->size_a / 2 >= push_index && stacks->size_b / 2 >= suit_index_in_b)
		return (pattern_a(stacks, push_index, suit_index_in_b, 0));
	else if (stacks->size_a / 2 <= push_index && stacks->size_b / 2 <= suit_index_in_b)
		return (pattern_b(stacks, push_index, suit_index_in_b, 0));
	else if (stacks->size_a / 2 >= (push_index + 1) && stacks->size_b / 2 <= (suit_index_in_b + 1))
		return (pattern_c(stacks, push_index, suit_index_in_b, 0));
	else
		return (pattern_d(stacks, push_index, suit_index_in_b, 0));
}

void	calculate_steps(int index, t_stacks stacks, long maxnum_stack_b, long minnum_stack_b, int *min_steps, int *best_index)
{
	int	steps;
	int	suit_index_in_b;

	suit_index_in_b = get_suit_index(stacks, maxnum_stack_b, best_index, min_steps, index);
	if (stacks.a[index].num > maxnum_stack_b)
		suit_index_in_b = pattern_max_num(&stacks, &maxnum_stack_b);
	if (stacks.a[index].num < minnum_stack_b)
		suit_index_in_b = pattern_min_num(&stacks, &minnum_stack_b);
	if (stacks.size_a / 2 >= index && stacks.size_b / 2 >= suit_index_in_b)
		steps = which_bigger(index, suit_index_in_b);
	else if (stacks.size_a / 2 <= index && stacks.size_b / 2 <= suit_index_in_b)
		steps = which_biggest(stacks, index, suit_index_in_b);
	else if (stacks.size_a / 2 >= (index + 1) && stacks.size_b / 2 <= (suit_index_in_b + 1))
		steps = which_is_smallnum(index + (stacks.size_b - suit_index_in_b), which_bigger(index, suit_index_in_b), which_bigger(stacks.size_a - index, stacks.size_b - suit_index_in_b));
	else if (stacks.size_a / 2 <= (index + 1) && stacks.size_b / 2 >= (suit_index_in_b + 1))
		steps = which_is_smallnum(suit_index_in_b + (stacks.size_a - index), which_bigger(index, suit_index_in_b), which_bigger(stacks.size_a - index, stacks.size_b - suit_index_in_b));
	if (*min_steps > steps || *min_steps == -1)
	{
		*min_steps = steps;
		*best_index = index;
	}
}

int	turk_sort_3(t_stacks stacks, int sizeof_args, long maxnum_stack_b)
{
	long	max_num;

	if (stacks.size_b > 1 && maxnum_b_index(stacks, maxnum_stack_b) < stacks.size_b / 2)
		while (stacks.size_b > 1 && stacks.b[0].num != maxnum_stack_b)
			rb(&stacks, 0, 0);
	else
		while (stacks.size_b > 1 && stacks.b[0].num != maxnum_stack_b)
			rrb(&stacks, 0, 0);
	max_num = is_biggest(stacks.a[stacks.size_a - 1].num, stacks.b[0].num, stacks.b[0].num);
	if (stacks.a[stacks.size_a - 1].num == max_num)
		rra(&stacks, 0, 0);
	while (stacks.size_b > 0 && !check_sorted(stacks, sizeof_args))
	{
		if (stacks.a[stacks.size_a - 1].num < stacks.b[0].num || (stacks.a[stacks.size_a - 1].num == max_num))
			pa(&stacks, 0);
		else
			rra(&stacks, 0, 0);
	}
	while (stacks.a[stacks.size_a - 1].num != max_num && !check_sorted(stacks, sizeof_args))
		rra(&stacks, 0, 0);
	return (check_sorted(stacks, sizeof_args));
}

int	turk_sort_2(t_stacks stacks, int sizeof_args)
{
	long	maxnum_stack_b;
	long	minnum_stack_b;
	int		push_index;

	maxnum_stack_b = stacks.b[0].num;
	minnum_stack_b = stacks.b[1].num;
	while (stacks.a[0].num > maxnum_stack_b && stacks.size_a > 4)
	{
		maxnum_stack_b = stacks.a[0].num;
		pb(&stacks, 0);
	}
	while (stacks.size_a > 5 && !check_sorted_a(stacks, stacks.size_a, 0, 0, 0))
	{
		push_index = choose_push_index(stacks, maxnum_stack_b, minnum_stack_b);
		push_to_b(&stacks, push_index, &maxnum_stack_b, &minnum_stack_b, 0);
	}
	if (check_sorted_a(stacks, stacks.size_a, 0, 0, 0))
		case_sorted_a(&stacks);
	if (stacks.size_a == 5)
		sort_ele5(&stacks);
	else if (stacks.size_a == 4)
		sort_ele4(&stacks);
	return (turk_sort_3(stacks, sizeof_args, maxnum_stack_b));
}

int	turk_sort(int sizeof_args, t_stacks stacks)
{
	if (check_sorted(stacks, sizeof_args))
		return (0);
	if (stacks.size_a == 2 && stacks.a[0].num > stacks.a[1].num)
		return (sa(&stacks, 0, 0), 0);
	if (stacks.size_a == 3)
		return (sort_ele3(&stacks), 0);
	if (stacks.size_a == 4)
		return (sort_ele4(&stacks), 0);
	if (stacks.size_a == 5)
		return (sort_ele5(&stacks), 0);
	else if (stacks.size_a > 5)
	{
		pb(&stacks, 0);
		pb(&stacks, 0);
		if (stacks.b[0].num < stacks.b[1].num)
			sb(&stacks, 0, 0);
	}
	if (!turk_sort_2(stacks, sizeof_args))
		return (1);
	else
		return (0);
}
