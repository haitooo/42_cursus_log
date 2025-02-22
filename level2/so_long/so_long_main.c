/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 14:15:39 by haito             #+#    #+#             */
/*   Updated: 2025/02/22 15:59:56 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	start_game(t_deta *deta)
{
	import_texture(deta->win, deta->tex);
	mlx_hook(deta->win->wind, X_CLICK, 0, key_esc, deta);
	mlx_loop_hook(deta->win->mlx, &draw_map, deta);
	mlx_loop(deta->win->mlx);
}

void	setting_key_hook(t_window *win, t_texture *tex, t_map *map, t_st *st)
{
	t_deta	*deta;

	deta = malloc(sizeof(t_deta));
	deta->win = win;
	deta->tex = tex;
	deta->map = map;
	deta->st = st;
	st->count_got_item = 0;
	st->count_step = 0;
	mlx_key_hook(win->wind, (void *)call_keypress_func, deta);
	start_game(deta);
}

void	import_texture(t_window *win, t_texture *tex)
{
	tex->back_img = mlx_xpm_file_to_image(win->mlx, TEX_BACK,
			&tex->img_width, &tex->img_height);
	if (!tex->back_img)
		return ;
	tex->player_up_img = mlx_xpm_file_to_image(win->mlx, TEX_PLAYER_UP,
			&tex->img_width, &tex->img_height);
	if (!tex->player_up_img)
		return ;
	tex->player_down_img = mlx_xpm_file_to_image(win->mlx, TEX_PLAYER_DOWN,
			&tex->img_width, &tex->img_height);
	if (!tex->player_down_img)
		return ;
	tex->player_left_img = mlx_xpm_file_to_image(win->mlx, TEX_PLAYER_LEFT,
			&tex->img_width, &tex->img_height);
	if (!tex->player_left_img)
		return ;
	tex->player_right_img = mlx_xpm_file_to_image(win->mlx, TEX_PLAYER_RIGHT,
			&tex->img_width, &tex->img_height);
	if (!tex->player_right_img)
		return ;
	tex->exit_img = mlx_xpm_file_to_image(win->mlx, TEX_EXIT,
			&tex->img_width, &tex->img_height);
	if (!tex->exit_img)
		return ;
	tex->item_img = mlx_xpm_file_to_image(win->mlx, TEX_ITEM,
			&tex->img_width, &tex->img_height);
	if (!tex->item_img)
		return ;
	tex->wall_img = mlx_xpm_file_to_image(win->mlx, TEX_WALL,
			&tex->img_width, &tex->img_height);
	if (!tex->wall_img)
		return ;
}

void	*select_texture(t_texture *tex, t_map *map, char c)
{
	if (c == '0')
		return (tex->back_img);
	if (c == '1')
		return (tex->wall_img);
	if (c == 'P')
	{
		if (map->direction == UP)
			return (tex->player_up_img);
		if (map->direction == DOWN)
			return (tex->player_down_img);
		if (map->direction == LEFT)
			return (tex->player_left_img);
		if (map->direction == RIGHT)
			return (tex->player_right_img);
	}
	if (c == 'C')
		return (tex->item_img);
	if (c == 'E')
		return (tex->exit_img);
	return (NULL);
}

int	draw_map(t_deta *d)
{
	int		h;
	int		w;
	void	*t;

	d->tex->img_width = IMG_WIDTH;
	d->tex->img_height = IMG_HEIGHT;
	h = 0;
	w = 0;
	while (h < d->map->map_height && h < 10)
	{
		while (w < d-> map->map_width && w < 20)
		{
			t = select_texture(d->tex, d->map, d->map->objs[h][w]);
			mlx_put_image_to_window(d->win->mlx, d->win->wind,
				t, w * IMG_WIDTH, h * IMG_HEIGHT);
			w++;
		}
		w = 0;
		h++;
	}
	return (0);
}

void	make_window(t_window *win, t_map *map)
{
	// if (map->map_width > MAX_WIDTH / IMG_WIDTH
	// 	|| map->map_height > MAX_HEIGHT / IMG_HEIGHT)
	// {
	// 	win->win_w = DEFAULT_WIDTH;
	// 	win->win_h = DEFAULT_HEIGHT;
	// }
	// else
	// {
		win->win_w = map->map_width * IMG_WIDTH;
		win->win_h = map->map_height * IMG_HEIGHT;
	// }
	win->mlx = mlx_init();
	if (!win->mlx)
		return ;
	win->wind = mlx_new_window(win->mlx, win->win_w, win->win_h, "so_long");
	if (!win)
		return ;
}

int	main(int argc, char **argv)
{
	t_window	win;
	t_texture	tex;
	t_map		map;
	t_st		st;

	if (argc >= 3)
		return (1);
	init_objs_count(&map);
	check_map_error(&map, argv[1]);
	make_window(&win, &map);
	map.direction = DOWN;
	setting_key_hook(&win, &tex, &map, &st);
	free_objs(&map, map.map_height);
	return (0);
}
