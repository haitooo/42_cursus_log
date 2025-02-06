#include "push_swap.h"

int	pattern_max_num(t_stacks *stacks, long *maxnum_stack_b)
{
	int	suit_index_in_b;

	suit_index_in_b = 0;
	while (suit_index_in_b < stacks->sizeof_stack_b)
	{
		if (stacks->stack_b[suit_index_in_b] == *maxnum_stack_b)
			return (suit_index_in_b);
		suit_index_in_b++;
	}
	return (suit_index_in_b);
}

int	pattern_min_num(t_stacks *stacks, long *minnum_stack_b)
{
	int	suit_index_in_b;

	suit_index_in_b = 0;
	while (suit_index_in_b < stacks->sizeof_stack_b)
	{
		if (stacks->stack_b[suit_index_in_b] == *minnum_stack_b)
			return (suit_index_in_b + 1);
		suit_index_in_b++;
	}
	return (suit_index_in_b);
}

int	maxnum_b_index(t_stacks stacks, long maxnum_stack_b)
{
	int	index;

	index = 0;
	while (stacks.stack_b[index] != maxnum_stack_b)
		index++;
	return (index);
}
