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
				return (LONG_MIN);
			else
				return (LONG_MAX);
		}
	}
	return (result * is_nega);
}

int	which_biggest(t_stacks stacks, int index, int suit_index_in_b)
{
	if (index == 0)
		return (stacks.size_b - suit_index_in_b);
	if (stacks.size_a - index >= stacks.size_b - suit_index_in_b)
		return (stacks.size_a - (index - 1));
	else
		return (stacks.size_b - (suit_index_in_b - 1));
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
