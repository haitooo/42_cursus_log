/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_error_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 03:55:02 by haito             #+#    #+#             */
/*   Updated: 2025/02/13 03:55:28 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "checker_bonus.h"

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
				my_free(argv, argc);
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
			my_free(argv, argc);
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

	n = 0;
	while (argc > ++n)
	{
		i = 0;
		while (argv[n][i] != '\0')
		{
			if (ft_isdigit(argv[n][i]) == 0)
			{
				if (i == 0 && (argv[n][i] == '-' || argv[n][i] == '+')
					&& ft_strlen(argv[n]) != 1)
				{
					i++;
					continue ;
				}
				ft_printf("Error\n");
				my_free(argv, argc);
				exit(1);
			}
			i++;
		}
	}
	check_args_error_over(argc, argv);
}
