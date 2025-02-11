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

void	sort_smallsize(t_stacks *stacks)
{
	if (stacks->size_a == 7)
		sort_ele7(stacks);
	if (stacks->size_a == 6)
		sort_ele6(stacks);
	if (stacks->size_a == 5)
		sort_ele5(stacks);
	if (stacks->size_a == 4)
		sort_ele4(stacks);
	if (stacks->size_a == 3)
		sort_ele3(stacks);
	if (stacks->size_a == 2 && stacks->a[0].num > stacks->a[1].num)
		sa(stacks, 0, 0);
}
