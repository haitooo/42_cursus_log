#include "push_swap.h"

t_stacks	push_to_b_nonmove(t_stacks stacks, int push_index, long *maxnum_stack_b, long *minnum_stack_b, int non_print)
{
	int suit_index_in_b;

	suit_index_in_b = 0;
	while (suit_index_in_b < stacks.sizeof_stack_b)
	{
		if (suit_index_in_b > 0 && stacks.stack_a[push_index] < stacks.stack_b[suit_index_in_b - 1] && stacks.stack_a[push_index] > stacks.stack_b[suit_index_in_b])
			break ;
		suit_index_in_b++;
	}
	if (stacks.stack_a[push_index] > *maxnum_stack_b)
	{
		suit_index_in_b = pattern_max_num(&stacks, maxnum_stack_b);
		*maxnum_stack_b = stacks.stack_a[push_index];
	}
	if (stacks.stack_a[push_index] < *minnum_stack_b)
	{
		suit_index_in_b = pattern_min_num(&stacks, minnum_stack_b);
		*minnum_stack_b = stacks.stack_a[push_index];
	}
	if (stacks.sizeof_stack_a / 2 >= push_index && stacks.sizeof_stack_b / 2 >= suit_index_in_b)
		return (pattern_a(&stacks, push_index, suit_index_in_b, non_print), stacks);
	else if (stacks.sizeof_stack_a / 2 <= push_index && stacks.sizeof_stack_b / 2 <= suit_index_in_b)
		return (pattern_b(&stacks, push_index, suit_index_in_b, non_print), stacks);
	else if (stacks.sizeof_stack_a / 2 >= (push_index + 1) && stacks.sizeof_stack_b / 2 <= (suit_index_in_b + 1))
		return (pattern_c(&stacks, push_index, suit_index_in_b, non_print), stacks);
	else if (stacks.sizeof_stack_a / 2 <= (push_index + 1) && stacks.sizeof_stack_b / 2 >= (suit_index_in_b + 1))
		return (pattern_d(&stacks, push_index, suit_index_in_b, non_print), stacks);
	return (stacks);
}
