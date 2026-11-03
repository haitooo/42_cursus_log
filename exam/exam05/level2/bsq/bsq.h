#ifndef BSQ_H
# define BSQ_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define SUCCESS 0
#define ERROR 1

typedef struct s_status {
	char**	map;
	int**	numbers_map;

	int		nof_row;
	int		nof_col;
	char	empty;
	char	obstacle;
	char	full;

	int		max_square;
	int		max_square_point[2];
} t_status;

int			read_map(FILE* fp, t_status* status);
int			validate_map(t_status* status);
int			run_map(t_status* status);
int			print_map(t_status* status);
void		free_status(t_status* status);



#endif

