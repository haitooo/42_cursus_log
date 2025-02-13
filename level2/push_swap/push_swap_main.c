/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 00:50:07 by haito             #+#    #+#             */
/*   Updated: 2025/02/13 20:05:55 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

t_date	*init_stack_a(int argc, char **argv, int n, int i)
{
	t_date	*date;
	long	min_num;

	date = (t_date *)malloc(sizeof(t_date) * (argc - 1));
	if (!date)
		return (NULL);
	while ((argc - 1) > n)
	{
		date[n].num = ft_atoi(argv[n + 1]);
		date[n++].coord = -1;
	}
	n = 0;
	while ((argc - 1) > n)
	{
		i = -1;
		min_num = EMPTY_INDEX_NUM;
		while (++i < (argc - 1))
			if (min_num > date[i].num && date[i].coord == -1)
				min_num = date[i].num;
		i = 0;
		while (date[i].num != min_num)
			i++;
		date[i].coord = n++;
	}
	return (date);
}

t_date	*init_stack_b(int argc)
{
	t_date	*date;
	int		n;

	n = 0;
	date = (t_date *)malloc(sizeof(t_date) * (argc - 1));
	if (!date)
		return (NULL);
	while ((argc - 1) > n)
	{
		date[n].num = EMPTY_INDEX_NUM;
		date[n].coord = -1;
		n++;
	}
	return (date);
}

int	main(int argc, char **argv)
{
	t_stacks	stacks;
	char		**nums;

	if (argc <= 1)
		return (0);
	nums = sep_argv(&argc, argv, -1, NULL);
	if (!nums)
		return (1);
	check_args_error(argc, nums);
	stacks.a = init_stack_a(argc, nums, 0, 0);
	if (!stacks.a)
		return (my_free(nums, argc), ft_printf("Error\n"), 1);
	stacks.b = init_stack_b(argc);
	if (!stacks.b)
		return (my_free(nums, argc), ft_printf("Error\n"), free(stacks.a), 1);
	my_free(nums, argc);
	stacks.size_a = argc - 1;
	stacks.size_b = 0;
	stacks.sizeof_args = argc -1;
	if (argc - 1 <= 300)
		turk_sort((argc - 1), stacks);
	else
		quick_sort((argc - 1), stacks, 0);
	return (free(stacks.a), free(stacks.b), 0);
}
