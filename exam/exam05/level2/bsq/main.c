#include "bsq.h"

int	main(int argc, char** argv)
{
	if (argc > 2)
	{
		printf("Error: Too many arguments\n");
		return (1);
	}

	t_status*	status = calloc(1, sizeof(t_status));
	if (status == NULL)
	{
		printf("Error: malloc failed\n");
		return (1);
	}

	FILE* fp = stdin;
	if (argc == 2)
	{
		fp = fopen(argv[1], "r");
		if (fp == NULL)
			return (printf("Error: cannot open file\n"), free_status(status), 1);
	}
	int ret = read_map(fp, status);
	if (fp != stdin)
		fclose(fp);
	if (ret == ERROR)
		return (free_status(status), 1);
	if (validate_map(status) == ERROR)
		return (free_status(status), 1);
	if (run_map(status) == ERROR)
		return (free_status(status), 1);
	if (print_map(status) == ERROR)
		return (free_status(status), 1);
	return (free_status(status), 0);
}
