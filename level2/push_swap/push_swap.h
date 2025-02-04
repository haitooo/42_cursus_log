#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include "ft_printf.h"

typedef struct s_stacks
{
	long	*stack_a;
	long	*stack_b;
	int		sizeof_stack_a;
	int		sizeof_stack_b;
}t_stacks;

size_t	ft_strlen(const char *s);
int		ft_isdigit(int c);
int		ft_atoi(const char *str);
long	ft_atol(const char *str);
int		is_smallnum(int index, int suit_index_in_b);
int		is_largenum(int index, int suit_index_in_b);
int		which_is_smallnum(int a, int b, int c);
void	check_args_error(int argc, char **argv);
void	check_args_error_over(int argc, char **argv);
void	check_args_error_duplicate(int argc, char **argv);
long	*init_stack_a(int argc, char **argv);
long	*init_stack_b(int argc);
int		push_swap_free(long	*hoge);
int		sort_stacks(int sizeof_args, long *stack_a, long *stack_b);
void	sa(t_stacks *stacks, int is_ss);
void	sb(t_stacks *stacks, int is_ss);
void	ss(t_stacks *stacks);
void	ra(t_stacks *stacks, int is_rr);
void	rb(t_stacks *stacks, int is_rr);
void	rr(t_stacks *stacks);
void	rra(t_stacks *stacks, int is_rrr);
void	rrb(t_stacks *stacks, int is_rrr);
void	rrr(t_stacks *stacks);
void	pa(t_stacks *stacks);
void	pb(t_stacks *stacks);

#endif