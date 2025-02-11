#include "push_swap.h"

t_date	*init_stack_a(int argc, char **argv, int n, int i, long min_num)
{
	t_date	*date;

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
		min_num = 9000000000;
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
		date[n].num = 9000000000;
		date[n].coord = -1;
		n++;
	}
	return (date);
}

int main(int argc, char **argv)
{
	t_stacks	stacks;
	char		**nums;

	if (argc <= 1)
		return (0);
	nums = sep_argv(&argc, argv, -1, NULL, NULL);
	check_args_error(argc, nums);
	stacks.a = init_stack_a(argc, nums, 0, 0, 9000000000);
	if (!stacks.a)
		return (my_free(nums, argc), ft_printf("Error\n"), 1);
	stacks.b = init_stack_b(argc);
	if (!stacks.b)
		return (my_free(nums, argc), ft_printf("Error\n"), free(stacks.a), 1);
	my_free(nums, argc);
	stacks.size_a = argc - 1;
	stacks.size_b = 0;
		if (argc - 1 <= 300)
			turk_sort((argc - 1), stacks);
		else
			quick_sort((argc - 1), stacks ,0);
	free(stacks.a);
	free(stacks.b);
	return (0);
}
