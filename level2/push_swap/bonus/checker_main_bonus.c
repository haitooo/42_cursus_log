/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_main_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 02:42:40 by haito             #+#    #+#             */
/*   Updated: 2025/02/13 20:07:19 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "checker_bonus.h"

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

int	do_action(t_stacks *stacks, char *line)
{
	if (!ft_strcmp(line, "ra\n\0"))
		return (ra(stacks, 0, 1), 0);
	if (!ft_strcmp(line, "rb\n\0"))
		return (rb(stacks, 0, 1), 0);
	if (!ft_strcmp(line, "rr\n\0"))
		return (rr(stacks, 1), 0);
	if (!ft_strcmp(line, "rra\n\0"))
		return (rra(stacks, 0, 1), 0);
	if (!ft_strcmp(line, "rrb\n\0"))
		return (rrb(stacks, 0, 1), 0);
	if (!ft_strcmp(line, "rrr\n\0"))
		return (rrr(stacks, 1), 0);
	if (!ft_strcmp(line, "sa\n\0"))
		return (sa(stacks, 0, 1), 0);
	if (!ft_strcmp(line, "sb\n\0"))
		return (sb(stacks, 0, 1), 0);
	if (!ft_strcmp(line, "ss\n\0"))
		return (ss(stacks, 1), 0);
	if (!ft_strcmp(line, "pa\n\0"))
		return (pa(stacks, 1), 0);
	if (!ft_strcmp(line, "pb\n\0"))
		return (pb(stacks, 1), 0);
	return (1);
}

void	read_stdin(t_stacks stacks)
{
	char	*line;

	while (1)
	{
		line = get_next_line(0);
		if (line == NULL)
			break ;
		if (do_action(&stacks, line))
		{
			ft_printf("Error\n");
			free(line);
			free(stacks.a);
			free(stacks.b);
			exit(1);
		}
		free(line);
	}
	if (check_sorted(stacks, stacks.sizeof_args))
		ft_printf("OK\n");
	else
		ft_printf("KO\n");
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
	read_stdin(stacks);
	free(stacks.a);
	free(stacks.b);
	return (0);
}
