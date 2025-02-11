#include "push_swap.h"

int	pattern_max_num(t_stacks *stacks, long *maxnum_stack_b)
{
	int	suit_index_in_b;

	suit_index_in_b = 0;
	while (suit_index_in_b < stacks->size_b)
	{
		if (stacks->b[suit_index_in_b].num == *maxnum_stack_b)
			return (suit_index_in_b);
		suit_index_in_b++;
	}
	return (suit_index_in_b);
}

int	pattern_min_num(t_stacks *stacks, long *minnum_stack_b)
{
	int	suit_index_in_b;

	suit_index_in_b = 0;
	while (suit_index_in_b < stacks->size_b)
	{
		if (stacks->b[suit_index_in_b].num == *minnum_stack_b)
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

int	choose_push_index(t_stacks stacks, long maxnum_stack_b, long minnum_stack_b)
{
	int	index;
	int	best_index;
	int	min_steps;

	index = 0;
	best_index = -1;
	min_steps = -1;
	while (index < stacks.size_a)
	{
		calculate_steps(index, stacks, maxnum_stack_b, minnum_stack_b, &min_steps, &best_index);
		index++;
	}
	if (best_index == -1 || min_steps == -1)
		return (ft_printf("Error\n", -1));
	return (best_index);
}
