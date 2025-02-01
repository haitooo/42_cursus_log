#include "push_swap.h"

void	check_args_error_duplicate(int argc, char **argv)
{
	int	n;
	int	i;
	int	num;

	n = 1;
	while (argc > n)
	{
		i = n + 1;
		num = ft_atoi(argv[n]);
		while (argc > i)
		{
			if (num == ft_atoi(argv[i]))
			{
				ft_printf("Error\n");
				exit(1);
			}
			i++;
		}
		n++;
	}
}

void	check_args_error_over(int argc, char **argv)
{
	int		n;
	long	num;

	n = 1;
	while (argc > n)
	{
		num = ft_atol(argv[n]);
		if (num > INT_MAX || num < INT_MIN)
		{
			ft_printf("Error\n");
			exit(1);
		}
		n++;
	}
	check_args_error_duplicate(argc, argv);
}

void	check_args_error(int argc, char **argv)
{
	int	n;
	int	i;

	n = 1;
	while (argc > n)
	{
		i = 0;
		while (argv[n][i] != '\0')
		{
			if (ft_isdigit(argv[n][i]) == 0)
			{
				if (i == 0 && argv[n][i++] == '-' && ft_strlen(argv[n]) != 1)
					continue;
				ft_printf("Error\n");
				exit(1);
			}
			i++;
		}
		n++;
	}
	check_args_error_over(argc, argv);
}
