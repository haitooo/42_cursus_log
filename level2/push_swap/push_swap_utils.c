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

int	is_smallnum(int index, int suit_index_in_b)
{
	if (index >= suit_index_in_b)
		return (suit_index_in_b);
	else
		return (index);
}

int	is_largenum(int index, int suit_index_in_b)
{
	if (index <= suit_index_in_b)
		return (suit_index_in_b);
	else
		return (index);
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
