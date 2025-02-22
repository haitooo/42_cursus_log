/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 14:15:42 by haito             #+#    #+#             */
/*   Updated: 2025/02/22 15:50:08 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 256
# endif

# include "ft_printf/ft_printf.h"
# include "ft_dprintf/ft_dprintf.h"

# include "key_num.h"
# include "error_num.h"
# include "texture.h"
# include <mlx.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <string.h>
# include <errno.h>

typedef struct s_window
{
	void	*mlx;
	void	*wind;
	int		win_w;
	int		win_h;
}	t_window;

typedef struct s_texture
{
	void	*back_img;
	void	*exit_img;
	void	*player_up_img;
	void	*player_down_img;
	void	*player_left_img;
	void	*player_right_img;
	void	*item_img;
	void	*wall_img;
	int		img_width;
	int		img_height;
}	t_texture;

typedef struct s_map
{
	int		map_width;
	int		map_height;
	int		x;
	int		y;
	int		direction;
	char	**objs;
	int		num_c;
	int		num_e;
	int		num_p;
}	t_map;

typedef struct s_status
{
	int	count_step;
	int	count_got_item;
}	t_st;

typedef struct s_deta
{
	t_window	*win;
	t_texture	*tex;
	t_map		*map;
	t_st		*st;
}	t_deta;

void	call_keypress_func(int keynum, t_deta *deta);
void	check_map_error(t_map *map, char *mapname);
void	init_objs_count(t_map *map);
int		count_object(t_map *map, int n);
char	**read_map(t_map *map, char *mapname);
char	*read_from_file(int fd);
void	import_texture(t_window *win, t_texture *tex);
int		draw_map(t_deta *d);

void	key_up(t_deta *d);
void	key_down(t_deta *d);
void	key_left(t_deta *d);
void	key_right(t_deta *d);
int		key_esc(t_deta *deta);

void	free_objs(t_map *map, int obj_size);
void	free_tmp_map(t_map *map, char **tmp_map);
void	error_map(t_map *map, int n, int errornum);
void	error_malloc(t_map *map, int n, char **tmp_map, int i);

size_t	ft_strlen(const char *s);
int		ft_int_strlen(const char *s);
char	**ft_split(const char *str, char c);
int		count_words(const char *str, char c);
char	*add_char(char *line, char c);

#endif