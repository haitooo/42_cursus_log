#include "push_swap.h"

void	ss(t_stacks *stacks, int non_print)
{
	sa(stacks, 1, 0);
	sb(stacks, 1, 0);
	if (!non_print)
		ft_printf("ss\n");
}

void	rr(t_stacks *stacks, int non_print)
{
	ra(stacks, 1, 0);
	rb(stacks, 1, 0);
	if (!non_print)
		ft_printf("rr\n");
}

void	rrr(t_stacks *stacks, int non_print)
{
	rra(stacks, 1, 0);
	rrb(stacks, 1, 0);
	if (!non_print)
		ft_printf("rrr\n");
}
