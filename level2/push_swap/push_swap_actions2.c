#include "push_swap.h"

void	rra(t_stacks *stacks, int is_rrr)
{
	long	tmp;
	int		i;

	i = 2;
	tmp = (stacks->stack_a)[stacks->sizeof_stack_a - 1];
	while (stacks->sizeof_stack_a - i >= 0)
	{
		(stacks->stack_a)[stacks->sizeof_stack_a - (i - 1)] = (stacks->stack_a)[stacks->sizeof_stack_a - i];
		i++;
	}
	(stacks->stack_a)[0] = tmp;
	if (!is_rrr)
		ft_printf("rra\n");
}

void	rrb(t_stacks *stacks, int is_rrr)
{
	long	tmp;
	int		i;

	i = 2;
	tmp = (stacks->stack_b)[stacks->sizeof_stack_b - 1];
	while (stacks->sizeof_stack_b - i >= 0)
	{
		(stacks->stack_b)[stacks->sizeof_stack_b - (i - 1)] = (stacks->stack_b)[stacks->sizeof_stack_b - i];
		i++;
	}
	(stacks->stack_b)[0] = tmp;
	if (!is_rrr)
		ft_printf("rrb\n");
}

void	pa(t_stacks *stacks)
{
	int	i;

	i = 0;
	if (stacks->sizeof_stack_b <=0)
		return ;
	while (i < stacks->sizeof_stack_a)
	{
		(stacks->stack_a)[stacks->sizeof_stack_a - i] = (stacks->stack_a)[stacks->sizeof_stack_a - 1 - i];
		i++;
	}
	(stacks->stack_a)[0] = (stacks->stack_b)[0];
	i = 0;
	while (i < (stacks->sizeof_stack_b - 1))
	{
		(stacks->stack_b)[i] = (stacks->stack_b)[i + 1];
		i++;
	}
	(stacks->stack_b)[i] = 9000000000;
	stacks->sizeof_stack_a += 1;
	stacks->sizeof_stack_b -= 1;
	ft_printf("pa\n");
}

void	pb(t_stacks *stacks)
{
	int	i;

	i = 0;
	if (stacks->sizeof_stack_a <=0)
		return ;
	while (i < stacks->sizeof_stack_b)
	{
		(stacks->stack_b)[stacks->sizeof_stack_b - i] = (stacks->stack_b)[stacks->sizeof_stack_b - 1 - i];
		i++;
	}
	(stacks->stack_b)[0] = (stacks->stack_a)[0];
	i = 0;
	while (i < (stacks->sizeof_stack_a - 1))
	{
		(stacks->stack_a)[i] = (stacks->stack_a)[i + 1];
		i++;
	}
	(stacks->stack_a)[i] = 9000000000;
	stacks->sizeof_stack_b += 1;
	stacks->sizeof_stack_a -= 1;
	ft_printf("pb\n");
}
