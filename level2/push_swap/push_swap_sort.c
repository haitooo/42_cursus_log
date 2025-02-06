#include "push_swap.h"

void	tester_print_stacks(long *stack_a, long *stack_b, int sizeof_args)
{
	int	n;

	ft_printf("\nstack_a:\n");
	n = 0;
	while (sizeof_args > n)
	{
		ft_printf("%l\n", stack_a[n]);
		n++;
	}
	ft_printf("stack_b:\n");
	n = 0;
	while (sizeof_args > n)
	{
		ft_printf("%l\n", stack_b[n]);
		n++;
	}
}

int	check_sorted(long *stack_a, int sizeof_args, int sizeof_stack_a)
{
	int	is_sorted;
	int	is_not_sorted;
	int	i;

	is_sorted = 1;
	is_not_sorted = 0;
	i = 0;
	if (sizeof_args != sizeof_stack_a)
		return (is_not_sorted);
	while (i < (sizeof_args - 1))
	{
		if (stack_a[i] > stack_a[i + 1])
			return (is_not_sorted);
		i++;
	}
	return (is_sorted);
}

int	push_to_b(t_stacks *stacks, int push_index, long *maxnum_stack_b, long *minnum_stack_b, int non_print)
{
	int suit_index_in_b;

	suit_index_in_b = 0;
	while (suit_index_in_b < stacks->sizeof_stack_b)
	{
		if (suit_index_in_b > 0 && stacks->stack_a[push_index] < stacks->stack_b[suit_index_in_b - 1] && stacks->stack_a[push_index] > stacks->stack_b[suit_index_in_b])
			break ;
		suit_index_in_b++;
	}
	if (stacks->stack_a[push_index] > *maxnum_stack_b)
	{
		suit_index_in_b = pattern_max_num(stacks, maxnum_stack_b);
		*maxnum_stack_b = stacks->stack_a[push_index];
	}
	if (stacks->stack_a[push_index] < *minnum_stack_b)
	{
		suit_index_in_b = pattern_min_num(stacks, minnum_stack_b);
		*minnum_stack_b = stacks->stack_a[push_index];
	}
	if (stacks->sizeof_stack_a / 2 >= push_index && stacks->sizeof_stack_b / 2 >= suit_index_in_b)
		return (pattern_a(stacks, push_index, suit_index_in_b, non_print));
	else if (stacks->sizeof_stack_a / 2 <= push_index && stacks->sizeof_stack_b / 2 <= suit_index_in_b)
		return (pattern_b(stacks, push_index, suit_index_in_b, non_print));
	else if (stacks->sizeof_stack_a / 2 >= (push_index + 1) && stacks->sizeof_stack_b / 2 <= (suit_index_in_b + 1))
		return (pattern_c(stacks, push_index, suit_index_in_b, non_print));
	else if (stacks->sizeof_stack_a / 2 <= (push_index + 1) && stacks->sizeof_stack_b / 2 >= (suit_index_in_b + 1))
		return (pattern_d(stacks, push_index, suit_index_in_b, non_print));
	return (ft_printf("push_to_b Error\n"));
}

int	calculate_steps(int index, t_stacks stacks, long maxnum_stack_b, long minnum_stack_b, int *min_steps, int *best_index)
{
	int	steps;
	int	suit_index_in_b;

	suit_index_in_b = 0;
	while (suit_index_in_b < stacks.sizeof_stack_b)
	{
		if (suit_index_in_b == 0 && stacks.stack_b[0] == maxnum_stack_b && stacks.stack_a[index] > maxnum_stack_b)
		{
			*best_index = 0;
			*min_steps = 0;
			return (*min_steps);
		}
		if (suit_index_in_b == 0 && stacks.stack_a[index] > stacks.stack_b[0] && stacks.stack_a[index] < stacks.stack_b[stacks.sizeof_stack_b - 1])
			break ;
		if (suit_index_in_b > 0 && stacks.stack_a[index] < stacks.stack_b[suit_index_in_b - 1] && stacks.stack_a[index] > stacks.stack_b[suit_index_in_b])
			break ;
		suit_index_in_b++;
	}
	if (stacks.stack_a[index] > maxnum_stack_b)
		suit_index_in_b = pattern_max_num(&stacks, &maxnum_stack_b);
	if (stacks.stack_a[index] < minnum_stack_b)
		suit_index_in_b = pattern_min_num(&stacks, &minnum_stack_b);
	if (stacks.sizeof_stack_a / 2 >= index && stacks.sizeof_stack_b / 2 >= suit_index_in_b)
		steps = is_largenum(index, suit_index_in_b);
	else if (stacks.sizeof_stack_a / 2 <= index && stacks.sizeof_stack_b / 2 <= suit_index_in_b)
		steps = is_smallnum(stacks, index, suit_index_in_b);
	else if (stacks.sizeof_stack_a / 2 >= (index + 1) && stacks.sizeof_stack_b / 2 <= (suit_index_in_b + 1))
		steps = which_is_smallnum(index + (stacks.sizeof_stack_b - suit_index_in_b), is_largenum(index, suit_index_in_b), is_largenum(stacks.sizeof_stack_a - index, stacks.sizeof_stack_b - suit_index_in_b));
	else if (stacks.sizeof_stack_a / 2 <= (index + 1) && stacks.sizeof_stack_b / 2 >= (suit_index_in_b + 1))
		steps = which_is_smallnum(suit_index_in_b + (stacks.sizeof_stack_a - index), is_largenum(index, suit_index_in_b), is_largenum(stacks.sizeof_stack_a - index, stacks.sizeof_stack_b - suit_index_in_b));
	if (*min_steps > steps || *min_steps == -1)
	{
		*min_steps = steps;
		*best_index = index;
	}

	//if (stacks.sizeof_stack_a > 5)
	//{
	//	t_stacks	stacks_cpy;
	//	int			min_steps_next;
	//	int			best_index_next;

	//	min_steps_next = -1;
	//	best_index_next = -1;
	//	stacks_cpy = push_to_b_nonmove(stacks, *best_index, &maxnum_stack_b, &minnum_stack_b, 1);
	//	return (steps + calculate_steps(*best_index, stacks_cpy, maxnum_stack_b, minnum_stack_b, &min_steps_next, &best_index_next))
	//}
	return (*min_steps);

	//ft_printf("debug index     : %d\n", index);
	//ft_printf("debug min_steps : %d\n", *min_steps);
	//ft_printf("debug steps     : %d\n", steps);
	//ft_printf("debug best_index: %d\n", *best_index);
	//ft_printf("debug suit_index: %d\n", suit_index_in_b);
	//ft_printf("debug maxnum_b  : %d\n", maxnum_stack_b);
	//ft_printf("debug sizeof_a  : %d\n", stacks.sizeof_stack_a);
	//ft_printf("debug sizeof_b  : %d\n\n", stacks.sizeof_stack_b);
}

int	choose_push_index(t_stacks stacks, long maxnum_stack_b, long minnum_stack_b)
{
	int	index;
	int	best_index;
	int	min_steps;

	index = 0;
	best_index = -1;
	min_steps = -1;
	while (index < stacks.sizeof_stack_a)
	{
		calculate_steps(index, stacks, maxnum_stack_b, minnum_stack_b, &min_steps, &best_index);
		//calculate_next_steps(index, stacks, maxnum_stack_b, minnum_stack_b, &min_steps, &best_index);
		index++;
	}
	if (best_index == -1 || min_steps == -1)
		return (ft_printf("coose_index Error\n", -1));
	return (best_index);
}

int	sort_stacks_2(t_stacks stacks, int sizeof_args)
{
	long	maxnum_stack_b;
	long	minnum_stack_b;
	long	max_num;
	int		push_index;

	maxnum_stack_b = stacks.stack_b[0];
	minnum_stack_b = stacks.stack_b[1];
	while (stacks.stack_a[0] > maxnum_stack_b && stacks.sizeof_stack_a > 4)
	{
		maxnum_stack_b = stacks.stack_a[0];
		pb(&stacks, 0);
	}
	//int	count = 0;
	//while (count < 13)
	while (stacks.sizeof_stack_a > 5)
	{
		push_index = choose_push_index(stacks, maxnum_stack_b, minnum_stack_b);
		push_to_b(&stacks, push_index, &maxnum_stack_b, &minnum_stack_b, 0);

		//count++;
		//ft_printf("\n");

	}
	if (stacks.sizeof_stack_a == 5)
		sort_ele5(&stacks);
	else if (stacks.sizeof_stack_a == 4)
		sort_ele4(&stacks);
	if (stacks.sizeof_stack_b > 1 && maxnum_b_index(stacks, maxnum_stack_b) < stacks.sizeof_stack_b / 2)
		while (stacks.sizeof_stack_b > 1 && stacks.stack_b[0] != maxnum_stack_b)
			rb(&stacks, 0, 0);
	else
		while (stacks.sizeof_stack_b > 1 && stacks.stack_b[0] != maxnum_stack_b)
			rrb(&stacks, 0, 0);
	max_num = is_biggest(stacks.stack_a[stacks.sizeof_stack_a - 1], stacks.stack_b[0], stacks.stack_b[0]);
	if (stacks.stack_a[stacks.sizeof_stack_a - 1] == max_num)
		rra(&stacks, 0, 0);
	while (stacks.sizeof_stack_b > 0 && !check_sorted(stacks.stack_a, sizeof_args, stacks.sizeof_stack_a))
	{
		if (stacks.stack_a[stacks.sizeof_stack_a - 1] < stacks.stack_b[0] || (stacks.stack_a[stacks.sizeof_stack_a - 1] == max_num))
			pa(&stacks, 0);
		else
			rra(&stacks, 0, 0);
	}
	while (stacks.stack_a[stacks.sizeof_stack_a - 1] != max_num && !check_sorted(stacks.stack_a, sizeof_args, stacks.sizeof_stack_a))
		rra(&stacks, 0, 0);
	//tester_print_stacks(stacks.stack_a, stacks.stack_b, sizeof_args);
	return (check_sorted(stacks.stack_a, sizeof_args, stacks.sizeof_stack_a));
}

int	sort_stacks(int sizeof_args, long *stack_a, long *stack_b)
{
	t_stacks	stacks;

	stacks.stack_a = stack_a;
	stacks.stack_b = stack_b;
	stacks.sizeof_stack_a = sizeof_args;
	stacks.sizeof_stack_b = 0;
	if (check_sorted(stack_a, sizeof_args, stacks.sizeof_stack_a))
		return (0);
	if (stacks.sizeof_stack_a == 2 && stacks.stack_a[0] > stacks.stack_a[1])
		return (sa(&stacks, 0, 0), 0);
	if (stacks.sizeof_stack_a == 3)
		return (sort_ele3(&stacks), 0);
	if (stacks.sizeof_stack_a == 4)
		return (sort_ele4(&stacks), 0);
	if (stacks.sizeof_stack_a == 5)
		return (sort_ele5(&stacks), 0);
	if (stacks.sizeof_stack_a == 5)
		pb(&stacks, 0);
	else if (stacks.sizeof_stack_a > 5)
	{
		pb(&stacks, 0);
		pb(&stacks, 0);
		if (stack_b[0] < stack_b[1])
			sb(&stacks, 0, 0);
	}
	if (!sort_stacks_2(stacks, sizeof_args))
		return (1);
	else
		return (0);
}
