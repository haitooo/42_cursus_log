#ifndef BSQ_H
# define BSQ_H

# include <stdio.h>
# include <stdlib.h>

typedef struct s_map
{
	char	**grid;
	int		rows;
	int		columns;
	char	empty;
	char	obstacle;
	char	full;
}	t_map;

typedef struct s_square
{
	int	size;
	int	bottom_row;
	int	right_column;
}	t_square;

#endif
