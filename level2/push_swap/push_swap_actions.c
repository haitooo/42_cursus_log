#include "push_swap.h"

void	sa(t_stacks *stacks, int is_ss, int non_print)
{
	long	tmp;

	if (stacks->sizeof_stack_a <= 1)
		return ;
	tmp = (stacks->stack_a)[0];
	(stacks->stack_a)[0] = (stacks->stack_a)[1];
	(stacks->stack_a)[1] = tmp;
	if (!is_ss && !non_print)
		ft_printf("sa\n");
}

void	sb(t_stacks *stacks, int is_ss, int non_print)
{
	long	tmp;

	if (stacks->sizeof_stack_b <= 1)
		return ;
	tmp = (stacks->stack_b)[0];
	(stacks->stack_b)[0] = (stacks->stack_b)[1];
	(stacks->stack_b)[1] = tmp;
	if (!is_ss && !non_print)
		ft_printf("sb\n");
}

void	ra(t_stacks *stacks, int is_rr, int non_print)
{
	int		i;
	long	tmp;

	i = 0;
	if (stacks->sizeof_stack_a <= 1)
		return;
	tmp = (stacks->stack_a)[0];
	while (i < (stacks->sizeof_stack_a - 1))
	{
		(stacks->stack_a)[i] = (stacks->stack_a)[i + 1];
		i++;
	}
	(stacks->stack_a)[i] = tmp;
	if (!is_rr && !non_print)
		ft_printf("ra\n");
}

void	rb(t_stacks *stacks, int is_rr, int non_print)
{
	int		i;
	long	tmp;

	i = 0;
	if (stacks->sizeof_stack_b <= 1)
		return;
	tmp = (stacks->stack_b)[0];
	while (i < (stacks->sizeof_stack_b - 1))
	{
		(stacks->stack_b)[i] = (stacks->stack_b)[i + 1];
		i++;
	}
	(stacks->stack_b)[i] = tmp;
	if (!is_rr && !non_print)
		ft_printf("rb\n");
}

