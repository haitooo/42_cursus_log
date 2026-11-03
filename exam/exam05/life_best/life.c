#include "life.h"

static void	free_cells(char **cells, int height)
{
	if (!cells)
		return ;
	for (int row = 0; row < height; row++)
		free(cells[row]);
	free(cells);
}

// calloc で確保するので、全セルが 0 (死んでいる状態) で始まる
static char	**allocate_cells(int width, int height)
{
	char	**cells = calloc(height, sizeof(char *));

	if (!cells)
		return (NULL);
	for (int row = 0; row < height; row++)
	{
		cells[row] = calloc(width, sizeof(char));
		if (!cells[row])
			return (free_cells(cells, height), NULL);
	}
	return (cells);
}

static void	draw_from_input(t_board *board)
{
	int		row = 0;
	int		column = 0;
	int		is_drawing = 0;
	char	command;

	while (read(STDIN_FILENO, &command, 1) == 1)
	{
		if (command == 'w' && row > 0)
			row--;
		else if (command == 's' && row < board->height - 1)
			row++;
		else if (command == 'a' && column > 0)
			column--;
		else if (command == 'd' && column < board->width - 1)
			column++;
		else if (command == 'x')
			is_drawing = !is_drawing;
		if (is_drawing)
			board->cells[row][column] = 1;
	}
}

static int	count_alive_neighbors(t_board *board, int row, int column)
{
	int	count = 0;

	for (int neighbor_row = row - 1; neighbor_row <= row + 1; neighbor_row++)
	{
		for (int neighbor_column = column - 1; neighbor_column <= column + 1; neighbor_column++)
		{
			if (neighbor_row == row && neighbor_column == column)
				continue ;
			if (neighbor_row < 0 || neighbor_row >= board->height
				|| neighbor_column < 0 || neighbor_column >= board->width)
				continue ;
			count += board->cells[neighbor_row][neighbor_column];
		}
	}
	return (count);
}

static void	advance_one_generation(t_board *board)
{
	char	**previous_cells;

	for (int row = 0; row < board->height; row++)
	{
		for (int column = 0; column < board->width; column++)
		{
			int	alive_neighbors = count_alive_neighbors(board, row, column);
			int	is_alive = board->cells[row][column];

			board->next_cells[row][column] = (alive_neighbors == 3
					|| (is_alive && alive_neighbors == 2));
		}
	}
	previous_cells = board->cells;
	board->cells = board->next_cells;
	board->next_cells = previous_cells;
}

static void	print_board(t_board *board)
{
	for (int row = 0; row < board->height; row++)
	{
		for (int column = 0; column < board->width; column++)
		{
			if (board->cells[row][column])
				putchar(ALIVE_CHARACTER);
			else
				putchar(DEAD_CHARACTER);
		}
		putchar('\n');
	}
}

int	main(int argc, char **argv)
{
	t_board	board = {0};
	int		iterations;

	if (argc != 4)
		return (1);
	board.width = atoi(argv[1]);
	board.height = atoi(argv[2]);
	iterations = atoi(argv[3]);
	if (board.width <= 0 || board.height <= 0 || iterations < 0)
		return (1);
	board.cells = allocate_cells(board.width, board.height);
	board.next_cells = allocate_cells(board.width, board.height);
	if (!board.cells || !board.next_cells)
	{
		free_cells(board.cells, board.height);
		free_cells(board.next_cells, board.height);
		return (1);
	}
	draw_from_input(&board);
	for (int generation = 0; generation < iterations; generation++)
		advance_one_generation(&board);
	print_board(&board);
	free_cells(board.cells, board.height);
	free_cells(board.next_cells, board.height);
	return (0);
}
