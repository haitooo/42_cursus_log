/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 14:15:42 by haito             #+#    #+#             */
/*   Updated: 2025/02/28 09:15:13 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 256
# endif

# define PEACE 0
# define EASY 1
# define HARD 2
// # define MODE PEACE
// # define MODE EASY
# define MODE HARD

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

typedef struct s_tex_num
{
	void	*zero_img;
	void	*one_img;
	void	*two_img;
	void	*three_img;
	void	*four_img;
	void	*five_img;
	void	*six_img;
	void	*seven_img;
	void	*eight_img;
	void	*nine_img;
	void	*status_img;
	void	*status1_img;
	void	*status2_img;
	void	*status3_img;
	void	*status4_img;
	void	*status5_img;
	void	*status6_img;
	void	*par_img;
	void	*item1_img;
	void	*item2_img;
	void	*item3_img;
	void	*item4_img;
	void	*item5_img;
}	t_tex_num;

typedef struct s_texture
{
	void		*back_img;
	void		*exit_close_img;
	void		*exit_open_img;
	void		*wall_img;
	void		*player_up_img;
	void		*player_down_img;
	void		*player_down_eye_img;
	void		*player_left_img;
	void		*player_right_img;
	void		*heart_img;
	void		*heart_half_img;
	void		*heart_zero_img;
	void		*trap1_img;
	void		*trap2_img;
	void		*enemy_stop_img;
	void		*enemy_move1_img;
	void		*enemy_move2_img;
	int			img_width;
	int			img_height;
	int			status_width;
	int			status_height;
	t_tex_num	*n;
}	t_texture;

typedef struct s_map
{
	int		map_width;
	int		map_height;
	int		x;
	int		y;
	int		direction;
	int		current_map_x;
	int		current_map_y;
	char	**objs;
	int		num_c;
	int		num_e;
	int		num_p;
	int		num_m;
	int		exit_x;
	int		exit_y;
	int		pat_x;
	int		pat_y;
	int		pat_direction;
}	t_map;

typedef struct s_status
{
	int				count_step;
	int				count_got_item;
	unsigned long	count_frame;
	int				hp;
	int				h;
	int				w;
	int				end_x;
	int				end_y;
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
int		count_object(t_map *map, int n, int i);
char	**read_map(t_map *map, char *mapname);
char	*read_from_file(int fd);
void	check_isolated_obj(t_map *map);
void	select_difficulty(t_st *st, t_window *win);
void	put_enemy(t_map *map);
void	init_texture(t_texture *t);
void	import_texture(t_deta *d);
int		import_numbers(t_window *win, t_texture *tex);
int		import_heart(t_window *win, t_texture *tex);
int		import_enemy(t_window *win, t_texture *tex);
int		import_item(t_window *win, t_texture *tex);
int		draw_map(t_deta *d);
void	draw_status(t_deta *d);
void	draw_heart(t_deta *d);
void	*select_num_tex(t_deta *d, int num);
void	*select_status_tex(t_texture *tex, int count);
void	*select_status2_tex(t_texture *tex, int count);
void	*select_tex(t_deta *d, char c);
void	*select_heart_tex(t_deta *d, int count);
void	search_player(t_map *map);
void	*case_player(t_deta *d);
void	*case_enemy(t_deta *d);
void	*case_trap(t_deta *d);
void	*case_item(t_deta *d);

void	key_up(t_deta *d);
void	key_down(t_deta *d);
void	key_left(t_deta *d);
void	key_right(t_deta *d);
void	check_patrol(t_deta *d);
int		key_esc(t_deta *deta);

void	p_right(t_map *map);
void	p_left(t_map *map);
void	p_up(t_map *map);
void	p_down(t_map *map);

void	destroy_imgs(t_deta *deta);
void	free_objs(t_map *map, int obj_size);
void	free_tmp_map(t_map *map, char **tmp_map);
void	error_map(t_map *map, int n, int errornum);
void	error_malloc(t_map *map, int n, char **tmp_map, int i);
void	error_hook(t_window *win, t_map *map, t_deta *deta, int errnum);
void	error_import(t_deta *d);

size_t	ft_strlen(const char *s);
int		ft_int_strlen(const char *s);
int		my_power(int base, int exp);
int		ft_strcmp(const char *s1, const char *s2);
char	**ft_split(const char *str, char c);
int		count_words(const char *str, char c);
char	*add_char(char *line, char c);

#endif