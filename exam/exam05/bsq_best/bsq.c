#include "bsq.h"

static int	is_printable(char character)
{
	return (character >= 32 && character <= 126);
}

static int	minimum_of_three(int first, int second, int third)
{
	int	minimum = first;

	if (second < minimum)
		minimum = second;
	if (third < minimum)
		minimum = third;
	return (minimum);
}

static void	free_map(t_map *map)
{
	for (int row = 0; map->grid && map->grid[row]; row++)
		free(map->grid[row]);
	free(map->grid);
}

// 1 行目は "<行数><空き文字><障害物文字><塗りつぶし文字>\n"
static int	read_header(FILE *file, t_map *map)
{
	char	*line = NULL;
	size_t	capacity = 0;
	ssize_t	length = getline(&line, &capacity, file);
	ssize_t	digit_count = length - 4;

	if (length < 5 || line[length - 1] != '\n')
		return (free(line), -1);
	map->empty = line[digit_count];
	map->obstacle = line[digit_count + 1];
	map->full = line[digit_count + 2];
	for (ssize_t i = 0; i < digit_count; i++)
	{
		if (line[i] < '0' || line[i] > '9' || map->rows > 100000000)
			return (free(line), -1);
		map->rows = map->rows * 10 + line[i] - '0';
	}
	free(line);
	if (map->rows <= 0
		|| !is_printable(map->empty) || !is_printable(map->obstacle)
		|| !is_printable(map->full)
		|| map->empty == map->obstacle || map->empty == map->full
		|| map->obstacle == map->full)
		return (-1);
	return (0);
}

static int	read_grid(FILE *file, t_map *map)
{
	map->grid = calloc(map->rows + 1, sizeof(char *));
	if (!map->grid)
		return (-1);
	for (int row = 0; row < map->rows; row++)
	{
		char	*line = NULL;
		size_t	capacity = 0;
		ssize_t	length = getline(&line, &capacity, file);

		if (length <= 0)
			return (free(line), -1);
		if (line[length - 1] == '\n')
			line[--length] = '\0';
		map->grid[row] = line;
		if (length == 0)
			return (-1);
		if (row == 0)
			map->columns = length;
		if (length != map->columns)
			return (-1);
		for (int column = 0; column < length; column++)
			if (line[column] != map->empty && line[column] != map->obstacle)
				return (-1);
	}
	return (0);
}

// square_sizes[row][column] は (row, column) を右下の角とする最大の正方形の一辺。
// 上から左へ走査し、より大きいときだけ更新するので、同じ大きさなら上・左のものが残る。
static int	find_biggest_square(t_map *map, t_square *biggest)
{
	int	*square_sizes = calloc(map->rows * map->columns, sizeof(int));

	if (!square_sizes)
		return (-1);
	for (int row = 0; row < map->rows; row++)
	{
		for (int column = 0; column < map->columns; column++)
		{
			int	index = row * map->columns + column;
			int	size = 0;

			if (map->grid[row][column] == map->empty)
			{
				if (row == 0 || column == 0)
					size = 1;
				else
					size = 1 + minimum_of_three(
							square_sizes[index - map->columns],
							square_sizes[index - 1],
							square_sizes[index - map->columns - 1]);
			}
			square_sizes[index] = size;
			if (size > biggest->size)
			{
				biggest->size = size;
				biggest->bottom_row = row;
				biggest->right_column = column;
			}
		}
	}
	free(square_sizes);
	return (0);
}

static void	fill_square(t_map *map, t_square *square)
{
	int	top_row = square->bottom_row - square->size + 1;
	int	left_column = square->right_column - square->size + 1;

	for (int row = top_row; row <= square->bottom_row; row++)
		for (int column = left_column; column <= square->right_column; column++)
			map->grid[row][column] = map->full;
}

static void	print_map(t_map *map)
{
	for (int row = 0; row < map->rows; row++)
		printf("%s\n", map->grid[row]);
}

static int	solve(FILE *file)
{
	t_map		map = {0};
	t_square	biggest = {0};
	int			result = read_header(file, &map);

	if (result == 0)
		result = read_grid(file, &map);
	if (result == 0)
		result = find_biggest_square(&map, &biggest);
	if (result == 0)
	{
		fill_square(&map, &biggest);
		print_map(&map);
	}
	free_map(&map);
	return (result);
}

int	main(int argc, char **argv)
{
	FILE	*file = stdin;
	int		result;

	if (argc > 2)
		return (printf("Error: too many arguments\n"), 1);
	if (argc == 2 && !(file = fopen(argv[1], "r")))
		return (printf("Error: cannot open file\n"), 1);
	result = solve(file);
	if (file != stdin)
		fclose(file);
	if (result != 0)
		printf("Error: map error\n");
	return (result != 0);
}
