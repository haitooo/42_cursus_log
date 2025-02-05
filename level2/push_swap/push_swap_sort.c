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

int	push_to_b(t_stacks *stacks, int push_index, long *maxnum_stack_b, long *minnum_stack_b, int suit_index_in_b)
{
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
		return (pattern_a(stacks, push_index, suit_index_in_b));
	else if (stacks->sizeof_stack_a / 2 <= push_index && stacks->sizeof_stack_b / 2 <= suit_index_in_b)
		return (pattern_b(stacks, push_index, suit_index_in_b));
	else if (stacks->sizeof_stack_a / 2 >= (push_index + 1) && stacks->sizeof_stack_b / 2 <= (suit_index_in_b + 1))
		return (pattern_c(stacks, push_index, suit_index_in_b));
	else if (stacks->sizeof_stack_a / 2 <= (push_index + 1) && stacks->sizeof_stack_b / 2 >= (suit_index_in_b + 1))
		return (pattern_d(stacks, push_index, suit_index_in_b));

	// ft_printf("debug : %d\n", stacks->sizeof_stack_a / 2);
	// ft_printf("debug : %d\n", push_index);
	// ft_printf("debug : %d\n", stacks->sizeof_stack_b / 2);
	// ft_printf("debug : %d\n", suit_index_in_b);
	

	return (ft_printf("push_to_b Error\n"));
}

void	calculate_steps(int index, t_stacks stacks, long maxnum_stack_b, long minnum_stack_b, int *min_steps, int *best_index)
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
			return ;
		}
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
		steps = which_is_smallnum(index + (stacks.sizeof_stack_b - suit_index_in_b), suit_index_in_b, stacks.sizeof_stack_a - index);
	else if (stacks.sizeof_stack_a / 2 <= (index + 1) && stacks.sizeof_stack_b / 2 >= (suit_index_in_b + 1))
		steps = which_is_smallnum(suit_index_in_b + (stacks.sizeof_stack_a - index), index, stacks.sizeof_stack_b - suit_index_in_b);

	ft_printf("steps : %d\n", steps);

	if (*min_steps > steps || *min_steps == -1)
	{
		*min_steps = steps;
		*best_index = index;
	}


	ft_printf("debug index     : %d\n", index);
	ft_printf("debug min_steps : %d\n", *min_steps);
	ft_printf("debug steps     : %d\n", steps);
	ft_printf("debug best_index: %d\n", *best_index);
	ft_printf("debug suit_index: %d\n", suit_index_in_b);
	ft_printf("debug maxnum_b  : %d\n", maxnum_stack_b);
	ft_printf("debug sizeof_a  : %d\n", stacks.sizeof_stack_a);
	ft_printf("debug sizeof_b  : %d\n\n", stacks.sizeof_stack_b);

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
		index++;
	// ft_printf("debug index %d: %d\n", index, min_steps);
	// ft_printf("debug index %d: %d\n", index, best_index);
	}
	if (best_index == -1 || min_steps == -1)
		return (ft_printf("coose_index Error\n", -1));


	return (best_index);
}

int	sort_stacks_2(t_stacks stacks, int sizeof_args)
{
	long	maxnum_stack_b;
	long	minnum_stack_b;
	int		push_index;

	maxnum_stack_b = stacks.stack_b[0];
	minnum_stack_b = stacks.stack_b[1];
	while (stacks.stack_a[0] > maxnum_stack_b)
	{
		maxnum_stack_b = stacks.stack_a[0];
		pb(&stacks);
	}
	int count = 0;
	while (stacks.sizeof_stack_a != 0)
	{
		push_index = choose_push_index(stacks, maxnum_stack_b, minnum_stack_b);
		
		ft_printf("test:index %d\n", push_index);
		
		push_to_b(&stacks, push_index, &maxnum_stack_b, &minnum_stack_b, 0);
		count++;
	}

	while (stacks.stack_b[0] != maxnum_stack_b)
		rb(&stacks, 0);
	while (stacks.sizeof_stack_b > 0)
		pa(&stacks);

	tester_print_stacks(stacks.stack_a, stacks.stack_b, sizeof_args);

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
	pb(&stacks);
	pb(&stacks);
	if (stack_b[0] < stack_b[1])
		sb(&stacks, 0);
	if (!sort_stacks_2(stacks, sizeof_args))
		return (ft_printf("Dosen't sorted!\n"), 1);
	else
		return (ft_printf("Sorted!!"), 0);
}
