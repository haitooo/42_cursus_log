#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include "ft_printf.h"

size_t	ft_strlen(const char *s);
int		ft_isdigit(int c);
int		ft_atoi(const char *str);
long	ft_atol(const char *str);
void	check_args_error(int argc, char **argv);
void	check_args_error_over(int argc, char **argv);
void	check_args_error_duplicate(int argc, char **argv);\
long	*init_stack_a(int argc, char **argv);
long	*init_stack_b(int argc);
int		push_swap_free(long	*hoge);
int		sort_stacks(int sizeof_args, long *stack_a, long *stack_b);
void	sa(long **stack_a, int *sizeof_stack_a);
void	sb(long **stack_b, int *sizeof_stack_b);
void	ra(long **stack_a, int *sizeof_stack_a);
void	rb(long **stack_b, int *sizeof_stack_b);
void	rra(long **stack_a, int *sizeof_stack_a);
void	rrb(long **stack_b, int *sizeof_stack_b);
void	pa(long **stack_a, long **stack_b, int *sizeof_stack_a, int *sizeof_stack_b);
void	pb(long **stack_a, long **stack_b, int *sizeof_stack_a, int *sizeof_stack_b);

#endif