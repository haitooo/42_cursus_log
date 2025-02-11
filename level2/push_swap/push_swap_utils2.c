#include "push_swap.h"

int	which_bigger(int a, int b)
{
	if (a <= b)
		return (b);
	else
		return (a);
}

long	is_biggest(long a, long b, long stack_b_num)
{
	if (stack_b_num == 9000000000)
		return (a);
	if (a <= b)
		return (b);
	else
		return (a);
}

int	which_is_smallnum(int a, int b, int c)
{
	int	small_num;

	small_num = a;
	if (a > b)
		small_num = b;
	if (small_num > c)
		small_num = c;
	return (small_num);
}

long	get_min_num(t_date *date, int sizeof_stack)
{
	long	min_num;
	int		i;

	min_num = 900000000000;
	i = 0;
	while (i < sizeof_stack)
	{
		if (date[i].num < min_num)
			min_num = date[i].num;
		i++;
	}
	return (min_num);
}

long	get_max_num(t_date *date, int sizeof_stack)
{
	long	max_num;
	int		i;

	max_num = -900000000000;
	i = 0;
	while (i < sizeof_stack)
	{
		if (date[i].num > max_num)
			max_num = date[i].num;
		i++;
	}
	return (max_num);
}
