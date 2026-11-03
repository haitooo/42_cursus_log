#ifndef LIFE_H
# define LIFE_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# define ALIVE_CHARACTER 'O'
# define DEAD_CHARACTER ' '

typedef struct s_board
{
	char	**cells;
	char	**next_cells;
	int		width;
	int		height;
}	t_board;

#endif
