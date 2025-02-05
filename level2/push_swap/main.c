#include "push_swap.h"

long	*init_stack_a(int argc, char **argv)
{
	long	*stack_a;
	int		n;

	n = 0;
	stack_a = (long *)malloc(sizeof(long) * (argc - 1));
	if (!stack_a)
		return (NULL);
	while ((argc - 1) > n)
	{
		stack_a[n] = ft_atoi(argv[n + 1]);
		n++;
	}
	return (stack_a);
}

long	*init_stack_b(int argc)
{
	long	*stack_b;
	int		n;

	n = 0;
	stack_b = (long *)malloc(sizeof(long) * (argc - 1));
	if (!stack_b)
		return (NULL);
	while ((argc - 1) > n)
	{
		stack_b[n] = 9000000000;
		n++;
	}
	return (stack_b);
}

int main(int argc, char **argv)
{
	long	*stack_a;
	long	*stack_b;

	if (argc <= 1)
		return (0);
	check_args_error(argc, argv);
	if (argc == 2)
		return (0);
	stack_a = init_stack_a(argc, argv);
	if (!stack_a)
		return (1);
	stack_b = init_stack_b(argc);
	if (!stack_b)
		return (free(stack_a), 1);
	sort_stacks((argc - 1), stack_a, stack_b);
	free(stack_a);
	free(stack_b);
	return (0);
}
