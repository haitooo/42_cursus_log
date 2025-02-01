#include "push_swap.h"

void	tester_print_stacks(long *stack_a, long *stack_b, int sizeof_args)
{
	int	n;

	ft_printf("stack_a:\n");
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

int	sort_stacks(int sizeof_args, long *stack_a, long *stack_b)
{
	int	sizeof_stack_a;
	int	sizeof_stack_b;

	sizeof_stack_a = sizeof_args;
	sizeof_stack_b = 0;
	pb(&stack_a, &stack_b, &sizeof_stack_a, &sizeof_stack_b);
	pa(&stack_a, &stack_b, &sizeof_stack_a, &sizeof_stack_b);
	ra(&stack_a, &sizeof_stack_a);
	sa(&stack_a, &sizeof_stack_a);
	rra(&stack_a, &sizeof_stack_a);
	pb(&stack_a, &stack_b, &sizeof_stack_a, &sizeof_stack_b);
	pb(&stack_a, &stack_b, &sizeof_stack_a, &sizeof_stack_b);
	pb(&stack_a, &stack_b, &sizeof_stack_a, &sizeof_stack_b);
	rrb(&stack_b, &sizeof_stack_b);


	tester_print_stacks(stack_a, stack_b, sizeof_args);

	return (0);
}
