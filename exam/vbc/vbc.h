#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define OPE 100
#define PLUS 201
#define STAR 202
#define NUM 101

typedef struct s_lst
{
	int		type;
	int		ope;
	int		val;
	s_lst	*next;
	s_lst	*prev;
}	t_lst;

