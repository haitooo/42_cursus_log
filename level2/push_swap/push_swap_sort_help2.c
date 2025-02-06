#include "push_swap.h"

int	pattern_a(t_stacks *stacks, int push_index, int suit_index_in_b, int non_print)
{
	int	count;

	count = 0;
	while (count < push_index && count < suit_index_in_b && push_index != 0 && suit_index_in_b != 0)
	{
		rr(stacks, non_print);
		count++;
	}
	while (count < push_index || count < suit_index_in_b)
	{
		if (count < push_index && push_index != 0)
			ra(stacks, 0, non_print);
		if (count < suit_index_in_b && suit_index_in_b != 0)
			rb(stacks, 0, non_print);
		count++;
	}
	return (pb(stacks, non_print), 0);
}

int	pattern_b(t_stacks *stacks, int push_index, int suit_index_in_b, int non_print)
{
	int	count;

	count = 0;
	while (count < stacks->sizeof_stack_a - push_index && count < stacks->sizeof_stack_b - suit_index_in_b
		&& push_index != stacks->sizeof_stack_a && suit_index_in_b != stacks->sizeof_stack_b)
	{
		rrr(stacks, non_print);
		count++;
	}
	while (count < stacks->sizeof_stack_a - push_index || count < stacks->sizeof_stack_b - suit_index_in_b)
	{
		if (count < stacks->sizeof_stack_a - push_index && push_index != stacks->sizeof_stack_a && push_index != 0)
			rra(stacks, 0, non_print);
		if (count < stacks->sizeof_stack_b - suit_index_in_b && suit_index_in_b != stacks->sizeof_stack_b)
			rrb(stacks, 0, non_print);
		count++;
	}
	return (pb(stacks, non_print), 0);
}

int	pattern_c(t_stacks *stacks, int push_index, int suit_index_in_b, int non_print)
{
	int	count;

	count = 0;
	if (push_index + (stacks->sizeof_stack_b - suit_index_in_b) <= is_largenum(push_index, suit_index_in_b)
		&& push_index + (stacks->sizeof_stack_b - suit_index_in_b) <= is_largenum(stacks->sizeof_stack_a - push_index, stacks->sizeof_stack_b - suit_index_in_b))
	{
		while (count < push_index)
		{
			ra(stacks, 0, non_print);
			count++;
		}
		count = 0;
		while (count < stacks->sizeof_stack_b - suit_index_in_b)
		{
			rrb(stacks, 0, non_print);
			count++;
		}
		return (pb(stacks, non_print), 0);
	}
	if (is_largenum(push_index, suit_index_in_b) < is_largenum(stacks->sizeof_stack_a - push_index, stacks->sizeof_stack_b - suit_index_in_b))
		return (pattern_a(stacks, push_index, suit_index_in_b, non_print));
	else
		return (pattern_b(stacks, push_index, suit_index_in_b, non_print));
}

int	pattern_d(t_stacks *stacks, int push_index, int suit_index_in_b, int non_print)
{
	int	count;

	count = 0;
	if (suit_index_in_b + (stacks->sizeof_stack_a - push_index) <= is_largenum(push_index, suit_index_in_b)
		&& suit_index_in_b + (stacks->sizeof_stack_a - push_index) <= is_largenum(stacks->sizeof_stack_a - push_index, stacks->sizeof_stack_b - suit_index_in_b))
	{
		while (count < (stacks->sizeof_stack_a - push_index))
		{
			rra(stacks, 0, non_print);
			count++;
		}
		count = 0;
		while (count < suit_index_in_b)
		{
			rb(stacks, 0, non_print);
			count++;
		}
		return (pb(stacks, non_print), 0);
	}
	if (is_largenum(push_index, suit_index_in_b) < is_largenum(stacks->sizeof_stack_a - push_index, stacks->sizeof_stack_b - suit_index_in_b))
		return (pattern_a(stacks, push_index, suit_index_in_b, non_print));
	else
		return (pattern_b(stacks, push_index, suit_index_in_b, non_print));
}

void	sort_ele3(t_stacks *stacks)
{
	long	a;
	long	b;
	long	c;

	a = stacks->stack_a[0];
	b = stacks->stack_a[1];
	c = stacks->stack_a[2];
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
	int		count;
	int		min_index;
	long	min_num_a;

	count = 0;
	min_num_a = 9000000000;
	while (count < stacks->sizeof_stack_a)
	{
		if (stacks->stack_a[count] < min_num_a)
		{
			min_num_a = stacks->stack_a[count];
			min_index = count;
		}
		count++;
	}
	if (min_index == 1 && stacks->stack_a[2] < stacks->stack_a[1] && stacks->stack_a[2] < stacks->stack_a[3])
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
	int		count;
	int		min_index;
	long	min_num_a;

	count = 0;
	min_num_a = 9000000000;
	while (count < stacks->sizeof_stack_a)
	{
		if (stacks->stack_a[count] < min_num_a)
		{
			min_num_a = stacks->stack_a[count];
			min_index = count;
		}
		count++;
	}
	//if (min_index == 1 && stacks->stack_a[2] < stacks->stack_a[1] && stacks->stack_a[2] < stacks->stack_a[3])
	//{
	//	sa(stacks, 0);
	//	return ;
	//}
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
