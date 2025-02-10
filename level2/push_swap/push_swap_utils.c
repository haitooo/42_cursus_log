#include "push_swap.h"

size_t	ft_strlen(const char *s)
{
	int	count;

	count = 0;
	while (s[count])
		count++;
	return (count);
}

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
{
	int				is_nega;
	unsigned long	result;

	result = 0;
	is_nega = 1;
	while ((*str >= 9 && *str <= 13) || *str == ' ')
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			is_nega *= -1;
		str++;
	}
	while (ft_isdigit(*str))
	{
		result = result * 10 + *str++ - '0';
		if (result > LONG_MAX)
		{
			if (is_nega == -1)
				return ((int)LONG_MIN);
			else
				return ((int)LONG_MAX);
		}
	}
	return ((int)(result * is_nega));
}

long	ft_atol(const char *str)
{
	int				is_nega;
	unsigned long	result;

	result = 0;
	is_nega = 1;
	if (*str == '-')
	{
		is_nega = -1;
		str++;
	}
	while (ft_isdigit(*str))
	{
		result = result * 10 + *str++ - '0';
		if (result > LONG_MAX)
		{
			if (is_nega == -1)
				return (LONG_MIN);
			else
				return (LONG_MAX);
		}
	}
	return (result * is_nega);
}

int	is_smallnum(t_stacks stacks, int index, int suit_index_in_b)
{
	if (index == 0)
		return (stacks.size_b - suit_index_in_b);
	if (stacks.size_a - index >= stacks.size_b - suit_index_in_b)
		return (stacks.size_a - (index - 1));
	else
		return (stacks.size_b - (suit_index_in_b - 1));
}

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

// void	view_date(t_stacks stacks, int sizeof_args)
// {
// 	int	n;

// 	n = 0;
// 	ft_printf("<stack_a>\n");
// 	while (n < sizeof_args)
// 	{
// 		ft_printf("num %l : coord %d\n", stacks.a[n].num, stacks.a[n].coord);
// 		n++;
// 	}
// 	ft_printf("\n<stack_b>\n");
// 	n = 0;
// 	while (n < sizeof_args)
// 	{
// 		ft_printf("num %l : coord %d\n", stacks.b[n].num, stacks.b[n].coord);
// 		n++;
// 	}
// 	ft_printf("\nsize a :%d\n", stacks.size_a);
// 	ft_printf("size b :%d\n", stacks.size_b);
// }
