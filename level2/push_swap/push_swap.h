/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 00:57:04 by haito             #+#    #+#             */
/*   Updated: 2025/02/13 20:05:28 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include "ft_printf.h"

# define EMPTY_INDEX_NUM 9000000000

typedef struct s_date
{
	long	num;
	int		coord;
}t_date;

typedef struct s_stacks
{
	t_date	*a;
	t_date	*b;
	int		size_a;
	int		size_b;
	int		sizeof_args;
}t_stacks;

size_t	ft_strlen(const char *s);
int		ft_isdigit(int c);
int		ft_atoi(const char *str);
long	ft_atol(const char *str);
char	**ft_split(const char *str, char c);
char	*ft_strjoin(char *s1, const char *s2);
int		count_words(const char *str, char c);
char	**sep_argv(int *argc, char **argv, int n, char *str);
char	**my_free(char **ret, int n);
int		which_biggest(t_stacks stacks, int index, int suit_index_in_b);
int		bigger(int index, int suit_index_in_b);
long	is_biggest(long a, long b, long stack_b_num);
int		which_is_smallnum(int a, int b, int c);
long	get_min_num(t_date *date, int sizeof_stack);
long	get_max_num(t_date *stack, int sizeof_stack);
int		maxnum_b_index(t_stacks stacks, long maxnum_stack_b);
void	check_args_error(int argc, char **argv);
void	check_args_error_over(int argc, char **argv);
void	check_args_error_duplicate(int argc, char **argv);
t_date	*init_stack_a(int argc, char **argv, int n, int i);
t_date	*init_stack_b(int argc);
int		turk_sort(int sizeof_args, t_stacks stacks);
void	quick_sort(int sizeof_args, t_stacks stacks, int count);
void	sa(t_stacks *stacks, int is_ss, int non_print);
void	sb(t_stacks *stacks, int is_ss, int non_print);
void	ss(t_stacks *stacks, int non_print);
void	ra(t_stacks *stacks, int is_rr, int non_print);
void	rb(t_stacks *stacks, int is_rr, int non_print);
void	rr(t_stacks *stacks, int non_print);
void	rra(t_stacks *stacks, int is_rrr, int non_print);
void	rrb(t_stacks *stacks, int is_rrr, int non_print);
void	rrr(t_stacks *stacks, int non_print);
void	pa(t_stacks *stacks, int non_print);
void	pb(t_stacks *stacks, int non_print);
void	sort_smallsize(t_stacks *stacks);
int		pattern_max_num(t_stacks *stacks);
int		pattern_min_num(t_stacks *stacks);
int		pattern_a(t_stacks *stacks, int push_index,
			int suit_index_in_b, int non_print);
int		pattern_b(t_stacks *stacks, int push_index,
			int suit_index_in_b, int non_print);
int		pattern_c(t_stacks *stacks, int push_index,
			int suit_index_in_b, int non_print);
int		pattern_d(t_stacks *stacks, int push_index,
			int suit_index_in_b, int non_print);
int		choose_push_index(t_stacks stacks);
void	calculate_steps(int index, t_stacks stacks,
			int *min_steps, int *best_index);
void	case_sorted_a(t_stacks *stacks);
int		get_suit_index(t_stacks stacks, int *best_index,
			int *min_steps, int index);
int		check_sorted_a(t_stacks stacks, int sizeof_args, int i, int count);
int		check_sorted(t_stacks stacks, int sizeof_args);
int		quick_a(t_stacks *stacks, int push_index, int suit_index);
int		quick_b(t_stacks *stacks, int push_index, int suit_index);
int		quick_c(t_stacks *stacks, int push_index, int suit_index);
int		quick_d(t_stacks *stacks, int push_index, int suit_index);
void	case_zero(t_stacks *stacks, int sizeof_args);
void	get_min(t_stacks *stacks, long *min_num_a, int *min_index);
void	sort_ele6(t_stacks *stacks);
void	sort_ele5(t_stacks *stacks);
void	sort_ele4(t_stacks *stacks);
void	sort_ele3(t_stacks *stacks);

// void	view_date(t_stacks stacks, int sizeof_args);

#endif