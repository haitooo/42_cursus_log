#include "push_swap.h"

void	sa(t_stacks *stacks, int is_ss, int non_print)
{
	t_date	tmp;

	if (stacks->size_a <= 1)
		return ;
	tmp = (stacks->a)[0];
	(stacks->a)[0] = (stacks->a)[1];
	(stacks->a)[1] = tmp;
	if (!is_ss && !non_print)
		ft_printf("sa\n");
}

void	sb(t_stacks *stacks, int is_ss, int non_print)
{
	t_date	tmp;

	if (stacks->size_b <= 1)
		return ;
	tmp = (stacks->b)[0];
	(stacks->b)[0] = (stacks->b)[1];
	(stacks->b)[1] = tmp;
	if (!is_ss && !non_print)
		ft_printf("sb\n");
}

void	ra(t_stacks *stacks, int is_rr, int non_print)
{
	int		i;
	t_date	tmp;

	i = 0;
	if (stacks->size_a <= 1)
		return;
	tmp = (stacks->a)[0];
	while (i < (stacks->size_a - 1))
	{
		(stacks->a)[i] = (stacks->a)[i + 1];
		i++;
	}
	(stacks->a)[i] = tmp;
	if (!is_rr && !non_print)
		ft_printf("ra\n");
}

void	rb(t_stacks *stacks, int is_rr, int non_print)
{
	int		i;
	t_date	tmp;

	i = 0;
	if (stacks->size_b <= 1)
		return;
	tmp = (stacks->b)[0];
	while (i < (stacks->size_b - 1))
	{
		(stacks->b)[i] = (stacks->b)[i + 1];
		i++;
	}
	(stacks->b)[i] = tmp;
	if (!is_rr && !non_print)
		ft_printf("rb\n");
}
