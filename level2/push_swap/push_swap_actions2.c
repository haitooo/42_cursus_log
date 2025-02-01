#include "push_swap.h"

void	rra(long **stack_a, int *sizeof_stack_a)
{
	long	tmp;
	int		i;

	i = 2;
	tmp = (*stack_a)[*sizeof_stack_a - 1];
	while (*sizeof_stack_a - i >= 0)
	{
		(*stack_a)[*sizeof_stack_a - (i - 1)] = (*stack_a)[*sizeof_stack_a - i];
		i++;
	}
	(*stack_a)[0] = tmp;
}

void	rrb(long **stack_b, int *sizeof_stack_b)
{
	long	tmp;
	int		i;

	i = 2;
	tmp = (*stack_b)[*sizeof_stack_b - 1];
	while (*sizeof_stack_b - i >= 0)
	{
		(*stack_b)[*sizeof_stack_b - (i - 1)] = (*stack_b)[*sizeof_stack_b - i];
		i++;
	}
	(*stack_b)[0] = tmp;
}

void	pa(long **stack_a, long **stack_b, int *sizeof_stack_a, int *sizeof_stack_b)
{
	int	i;

	i = 0;
	if (*sizeof_stack_b <=0)
		return ;
	while (i < *sizeof_stack_a)
	{
		(*stack_a)[*sizeof_stack_a - i] = (*stack_a)[*sizeof_stack_a - 1 - i];
		i++;
	}
	(*stack_a)[0] = (*stack_b)[0];
	i = 0;
	while (i < (*sizeof_stack_b - 1))
	{
		(*stack_b)[i] = (*stack_b)[i + 1];
		i++;
	}
	(*stack_b)[i] = 9000000000;
	*sizeof_stack_a += 1;
	*sizeof_stack_b -= 1;
}

void	pb(long **stack_a, long **stack_b, int *sizeof_stack_a, int *sizeof_stack_b)
{
	int	i;

	i = 0;
	if (*sizeof_stack_a <=0)
		return ;
	while (i < *sizeof_stack_b)
	{
		(*stack_b)[*sizeof_stack_b - i] = (*stack_b)[*sizeof_stack_b - 1 - i];
		i++;
	}
	(*stack_b)[0] = (*stack_a)[0];
	i = 0;
	while (i < (*sizeof_stack_a - 1))
	{
		(*stack_a)[i] = (*stack_a)[i + 1];
		i++;
	}
	(*stack_a)[i] = 9000000000;
	*sizeof_stack_b += 1;
	*sizeof_stack_a -= 1;
}
