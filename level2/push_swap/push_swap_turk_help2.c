#include "push_swap.h"

int	pattern_a(t_stacks *stacks, int push_index, int suit_index_in_b, int non_print)
{
	int		count;

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
	int		count;

	count = 0;
	while (count < stacks->size_a - push_index && count < stacks->size_b - suit_index_in_b
		&& push_index != stacks->size_a && suit_index_in_b != stacks->size_b)
	{
		rrr(stacks, non_print);
		count++;
	}
	while (count < stacks->size_a - push_index || count < stacks->size_b - suit_index_in_b)
	{
		if (count < stacks->size_a - push_index && push_index != stacks->size_a && push_index != 0)
			rra(stacks, 0, non_print);
		if (count < stacks->size_b - suit_index_in_b && suit_index_in_b != stacks->size_b)
			rrb(stacks, 0, non_print);
		count++;
	}
	return (pb(stacks, non_print), 0);
}

int	pattern_c(t_stacks *stacks, int push_index, int suit_index_in_b, int non_print)
{
	int		count;

	count = 0;
	if (push_index + (stacks->size_b - suit_index_in_b) <= which_bigger(push_index, suit_index_in_b)
		&& push_index + (stacks->size_b - suit_index_in_b) <= which_bigger(stacks->size_a - push_index, stacks->size_b - suit_index_in_b))
	{
		while (count < push_index)
		{
			ra(stacks, 0, non_print);
			count++;
		}
		count = 0;
		while (count < stacks->size_b - suit_index_in_b)
		{
			rrb(stacks, 0, non_print);
			count++;
		}
		return (pb(stacks, non_print), 0);
	}
	if (which_bigger(push_index, suit_index_in_b) < which_bigger(stacks->size_a - push_index, stacks->size_b - suit_index_in_b))
		return (pattern_a(stacks, push_index, suit_index_in_b, non_print));
	else
		return (pattern_b(stacks, push_index, suit_index_in_b, non_print));
}

int	pattern_d(t_stacks *stacks, int push_index, int suit_index_in_b, int non_print)
{
	int		count;

	count = 0;
	if (suit_index_in_b + (stacks->size_a - push_index) <= which_bigger(push_index, suit_index_in_b)
		&& suit_index_in_b + (stacks->size_a - push_index) <= which_bigger(stacks->size_a - push_index, stacks->size_b - suit_index_in_b))
	{
		while (count < (stacks->size_a - push_index))
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
	if (which_bigger(push_index, suit_index_in_b) < which_bigger(stacks->size_a - push_index, stacks->size_b - suit_index_in_b))
		return (pattern_a(stacks, push_index, suit_index_in_b, non_print));
	else
		return (pattern_b(stacks, push_index, suit_index_in_b, non_print));
}

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

void	get_min(t_stacks *stacks, long *min_num_a, int *min_index)
{
	int	count;

	count = 0;
	while (count < stacks->size_a)
	{
		if (stacks->a[count].num < *min_num_a)
		{
			*min_num_a = stacks->a[count].num;
			*min_index = count;
		}
		count++;
	}
}

void	sort_ele4(t_stacks *stacks)
{
	int		min_index;
	long	min_num_a;

	min_index = -1;
	min_num_a = 9000000000;
	get_min(stacks, &min_num_a, &min_index);
	if (min_index == 1 && stacks->a[2].num < stacks->a[1].num && stacks->a[2].num < stacks->a[3].num)
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
