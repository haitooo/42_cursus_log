#include "bsq.h"

int			read_map(FILE* fp, t_status* status)
{
	char*	line = NULL;
	size_t	cap = 0;
	ssize_t	len;

	len = getline(&line, &cap, fp);
	if (len == -1 || len < 4)
	{
		free(line);
		if (len == -1)
			printf("Error: getline failed\n");
		else
			printf("Error: map error: few element in line\n");
		return (ERROR);
	}
	if (line[len - 1] == '\n')
		line[len - 1] = '\0';
	else
	{
		free(line);
		printf("Error: map error: header no has line break\n");
		return (ERROR);
	}
	status->full = line[len - 2];
	line[len - 2] = '\0';
	status->obstacle = line[len - 3];
	line[len - 3] = '\0';
	status->empty = line[len - 4];
	line[len - 4] = '\0';

	int	length = len - 4;
	int	j = 0;
	int	n;
	int	sum = 0;
	int	upper = 1;
	while (0 < length)
	{
		if (line[j] < '0' || line[j] > '9')
		{
			free(line);
			printf("Error: map error: the number of lines, not a positive number\n");
			return (ERROR);
		}
		n = 0;
		while (n < length - 1)
		{
			upper *= 10;
			n++;
		}
		sum += (line[j] - '0') * upper;
		j++;
		length--;
		upper = 1;
	}
	status->nof_row = sum;
	if (status->nof_row < 1)
	{
		free(line);
		printf("Error: map error: the number of lines, less than 1\n");
		return (ERROR);
	}
	free(line);

	status->map = calloc(status->nof_row + 1, sizeof(char *));
	if (status->map == NULL)
	{
		printf("Error: malloc failed\n");
		return (ERROR);
	}

	int i = 0;
	while (i < status->nof_row)
	{
		line = NULL;
		cap = 0;

		len = getline(&line, &cap, fp);
		if (len == -1 || line[len - 1] != '\n')
		{
			free(line);
			if (len == -1)
				printf("Error: getline failed\n");
			else
				printf("Error: map error: line no has a line break\n");
			return (ERROR);
		}
		line[len - 1] = '\0';
		if (i == 0)
			status->nof_col = len - 1;
		else
		{
			if (status->nof_col == 0 || len - 1 != status->nof_col)
			{
				free(line);
				printf("Error: map error: lines have different lengths\n");
				return (ERROR);
			}
		}
		status->map[i] = line;
		i++;
	}
	return (SUCCESS);
}

int	is_printable(char c)
{
	return (c >= ' ' && c <= '~');
}

int		validate_map(t_status* status)
{
	if (!is_printable(status->empty) || !is_printable(status->obstacle) || !is_printable(status->full))
	{
		printf("Error: map error: invalid character in header\n");
		return (ERROR);
	}
	if (status->empty == status->obstacle || status->empty == status->full
		|| status->obstacle == status->full)
	{
		printf("Error: map error: duplicate character in header\n");
		return (ERROR);
	}

	int	i = 0;
	int	j;
	while (i < status->nof_row)
	{
		j = 0;
		while (j < status->nof_col)
		{
			if ((status->map[i][j] != status->empty && status->map[i][j] != status->obstacle))
			{
				printf("Error: map error\n");
				return (ERROR);
			}
			j++;
		}
		i++;
	}
	return (SUCCESS);
}

int	fill_map(t_status *status)
{
	int	top = status->max_square_point[1] - status->max_square + 1;
	int	left = status->max_square_point[0] - status->max_square + 1;

	for (int i = top; i <= status->max_square_point[1]; i++)
		for (int j = left; j <= status->max_square_point[0]; j++)
			status->map[i][j] = status->full;
	return (SUCCESS);
}


int	ft_min(int a, int b, int c)
{
	int	m = a;
	if (m > b)
		m = b;
	if (m > c)
		m = c;
	return (m);
}

int	run_map(t_status *status)
{
	status->numbers_map = calloc(status->nof_row, sizeof(int *));
	if (status->numbers_map == NULL)
		return (printf("Error: malloc failed\n"), ERROR);
	for (int i = 0; i < status->nof_row; i++)
	{
		status->numbers_map[i] = calloc(status->nof_col, sizeof(int));
		if (status->numbers_map[i] == NULL)
			return (printf("Error: malloc failed\n"), ERROR);
	}
	for (int i = 0; i < status->nof_row; i++)
	{
		for (int j = 0; j < status->nof_col; j++)
		{
			if (status->map[i][j] == status->obstacle)
				status->numbers_map[i][j] = 0;
			else if (i == 0 || j == 0)
				status->numbers_map[i][j] = 1;
			else
				status->numbers_map[i][j] = ft_min(status->numbers_map[i - 1][j],
						status->numbers_map[i][j - 1],
						status->numbers_map[i - 1][j - 1]) + 1;
			if (status->numbers_map[i][j] > status->max_square)
			{
				status->max_square = status->numbers_map[i][j];
				status->max_square_point[0] = j;
				status->max_square_point[1] = i;
			}
		}
	}
	fill_map(status);
	return (SUCCESS);
}

int			print_map(t_status* status)
{
	int	i = 0;
	while (i < status->nof_row)
	{
		printf("%s\n", status->map[i]);
		i++;
	}
	return (SUCCESS);
}

void		free_status(t_status* status)
{
	if (status == NULL)
		return ;
	if (status->map != NULL)
	{
		for (int i = 0;status->map[i] != NULL; i++)
			free(status->map[i]);
		free(status->map);
	}
	if (status->numbers_map != NULL)
	{
		for (int i = 0; i < status->nof_row; i++)
			free(status->numbers_map[i]);
		free(status->numbers_map);
	}
	free(status);
}
