#include "push_swap.h"

int	quick_a(t_stacks *stacks, int push_index, int suit_index)
{
	int	count;

	count = 0;
	while (count < push_index && count < suit_index)
	{
		rr(stacks, 0);
		count++;
	}
	while (count < push_index || count < suit_index)
	{
		if (count < push_index)
			rb(stacks, 0, 0);
		else
			ra(stacks, 0, 0);
		count++;
	}
	return (pa(stacks, 0), 0);
}

int	quick_b(t_stacks *stacks, int push_index, int suit_index)
{
		int	count;

	count = 0;
	while (count < stacks->size_b - push_index && count < stacks->size_a - suit_index)
	{
		rrr(stacks, 0);
		count++;
	}
	while (count < stacks->size_b - push_index || count <  stacks->size_a - suit_index)
	{
		if (count < stacks->size_b - push_index)
			rrb(stacks, 0, 0);
		else
			rra(stacks, 0, 0);
		count++;
	}
	return (pa(stacks, 0), 0);
}

int	quick_c(t_stacks *stacks, int push_index, int suit_index)
{
	int		count;

	count = 0;
	if (suit_index + (stacks->size_b - push_index) <= which_bigger(push_index, suit_index)
		&& suit_index + (stacks->size_b - push_index) <= which_bigger(stacks->size_b - push_index, stacks->size_a - suit_index))
	{
		while (count < suit_index)
		{
			ra(stacks, 0, 0);
			count++;
		}
		count = 0;
		while (count < stacks->size_b - push_index)
		{
			rrb(stacks, 0, 0);
			count++;
		}
		return (pa(stacks, 0), 0);
	}
	if (which_bigger(push_index, suit_index) < which_bigger(stacks->size_b - push_index, stacks->size_a - suit_index))
		return (quick_a(stacks, push_index, suit_index));
	else
		return (quick_b(stacks, push_index, suit_index));
}

int	quick_d(t_stacks *stacks, int push_index, int suit_index)
{
	int		count;

	count = 0;
	if (push_index + (stacks->size_a - suit_index) <= which_bigger(push_index, suit_index)
		&& push_index + (stacks->size_a - suit_index) <= which_bigger(stacks->size_b - push_index, stacks->size_a - suit_index))
	{
		while (count < stacks->size_a - suit_index)
		{
			rra(stacks, 0, 0);
			count++;
		}
		count = 0;
		while (count < push_index)
		{
			rb(stacks, 0, 0);
			count++;
		}
		return (pa(stacks, 0), 0);
	}
	if (which_bigger(push_index, suit_index) < which_bigger(stacks->size_b - push_index, stacks->size_a - suit_index))
		return (quick_a(stacks, push_index, suit_index));
	else
		return (quick_b(stacks, push_index, suit_index));
}

void	case_zero(t_stacks *stacks, int sizeof_args)
{
	int	count;

	count = 0;
	while (stacks->b[count].coord != sizeof_args - 1)
		count++;
	if (count <= sizeof_args / 2)
		while (stacks->b[0].coord != sizeof_args - 1)
			rb(stacks, 0, 0);
	else
		while (stacks->b[0].coord != sizeof_args - 1)
			rrb(stacks, 0, 0);
	pa(stacks, 0);
}
