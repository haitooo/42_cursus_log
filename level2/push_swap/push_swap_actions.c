#include "push_swap.h"

void	sa(long **stack_a, int *sizeof_stack_a)
{
	long	tmp;

	if (*sizeof_stack_a <= 1)
		return ;
	tmp = (*stack_a)[0];
	(*stack_a)[0] = (*stack_a)[1];
	(*stack_a)[1] = tmp;
}

void	sb(long **stack_b, int *sizeof_stack_b)
{
	long	tmp;

	if (*sizeof_stack_b <= 1)
		return ;
	tmp = (*stack_b)[0];
	(*stack_b)[0] = (*stack_b)[1];
	(*stack_b)[1] = tmp;
}

void	ra(long **stack_a, int *sizeof_stack_a)
{
	int		i;
	long	tmp;

	i = 0;
	if (*sizeof_stack_a <= 1)
		return;
	tmp = (*stack_a)[0];
	while (i < (*sizeof_stack_a - 1))
	{
		(*stack_a)[i] = (*stack_a)[i + 1];
		i++;
	}
	(*stack_a)[i] = tmp;
}

void	rb(long **stack_b, int *sizeof_stack_b)
{
	int		i;
	long	tmp;

	i = 0;
	if (*sizeof_stack_b <= 1)
		return;
	tmp = (*stack_b)[0];
	while (i < (*sizeof_stack_b - 1))
	{
		(*stack_b)[i] = (*stack_b)[i + 1];
		i++;
	}
	(*stack_b)[i] = tmp;
}

