#include "push_swap.h"

int	pattern_a(t_stacks *stacks, int push_index, int suit_index_in_b)
{
	int	count;

	count = 0;
	while (count < push_index && count < suit_index_in_b && push_index != 0 && suit_index_in_b != 0)
	{
		rr(stacks);
		count++;
	}
	while (count < push_index || count < suit_index_in_b)
	{
		if (count < push_index && push_index != 0)
			ra(stacks, 0);
		if (count < suit_index_in_b && suit_index_in_b != 0)
			rb(stacks, 0);
		count++;
	}
	return (pb(stacks), 0);
}

int	pattern_b(t_stacks *stacks, int push_index, int suit_index_in_b)
{
	int	count;

	count = 0;
	while (count < stacks->sizeof_stack_a - push_index - 1 && count < stacks->sizeof_stack_b - suit_index_in_b - 1
		&& push_index != stacks->sizeof_stack_a - 1 && suit_index_in_b != stacks->sizeof_stack_b - 1)
	{
		rrr(stacks);
		count++;
	}
	while (count < stacks->sizeof_stack_a - push_index || count < stacks->sizeof_stack_b - suit_index_in_b)
	{
		if (count < stacks->sizeof_stack_a - push_index && push_index != stacks->sizeof_stack_a && push_index != 0)
			rra(stacks, 0);
		if (count < stacks->sizeof_stack_b - suit_index_in_b && suit_index_in_b != stacks->sizeof_stack_b)
			rrb(stacks, 0);
		count++;
	}
	return (pb(stacks), 0);
}

int	pattern_c(t_stacks *stacks, int push_index, int suit_index_in_b)
{
	int	count;

	count = 0;
	if ((push_index + (stacks->sizeof_stack_b - suit_index_in_b) <= suit_index_in_b
		&& push_index + (stacks->sizeof_stack_b - suit_index_in_b) <= stacks->sizeof_stack_a - push_index) || push_index == 0)
	{
		while (count < push_index)
		{
			ra(stacks, 0);
			count++;
		}
		count = 0;
		while (count < stacks->sizeof_stack_b - suit_index_in_b)
		{
			rrb(stacks, 0);
			count++;
		}
		return (pb(stacks), 0);
	}
	count = 0;
	if (suit_index_in_b < push_index + (stacks->sizeof_stack_b - suit_index_in_b) && suit_index_in_b < stacks->sizeof_stack_a - push_index)
		return (pattern_a(stacks, push_index, suit_index_in_b));
	if (stacks->sizeof_stack_a - push_index < push_index + (stacks->sizeof_stack_b - suit_index_in_b)
		&& stacks->sizeof_stack_a - push_index < suit_index_in_b)
		return (pattern_b(stacks, push_index, suit_index_in_b));
	return (-1);
}

int	pattern_d(t_stacks *stacks, int push_index, int suit_index_in_b)
{
	int	count;

	count = 0;
	if ((suit_index_in_b + (stacks->sizeof_stack_a - push_index) <= push_index
		&& suit_index_in_b + (stacks->sizeof_stack_a - push_index) <= stacks->sizeof_stack_b - suit_index_in_b) || push_index == 0)
	{
		while (count < push_index)
		{
			rra(stacks, 0);
			count++;
		}
		count = 0;
		while (count < stacks->sizeof_stack_b - suit_index_in_b)
		{
			rb(stacks, 0);
			count++;
		}
		return (pb(stacks), 0);
	}
	count = 0;
	if (push_index < suit_index_in_b + (stacks->sizeof_stack_a - push_index) && push_index < stacks->sizeof_stack_b - suit_index_in_b)
		return (pattern_b(stacks, push_index, suit_index_in_b));
	if (stacks->sizeof_stack_b - suit_index_in_b < suit_index_in_b + (stacks->sizeof_stack_a - push_index)
		&& stacks->sizeof_stack_b - suit_index_in_b < push_index)
		return (pattern_a(stacks, push_index, suit_index_in_b));
	return (-1);
}
