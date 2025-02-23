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
	tex->img_width = IMG_WIDTH;
	tex->img_height = IMG_HEIGHT;
	tex->status_width = STATUS_WIDTH;
	tex->status_height = STATUS_HEIGHT;
	map->current_map_x = (map->x / NUM_OF_TILEX) * NUM_OF_TILEX;
	map->current_map_y = (map->y / NUM_OF_TILEY) * NUM_OF_TILEY;
	mlx_key_hook(win->wind, (void *)call_keypress_func, deta);
	start_game(deta);
}

void	make_window(t_window *win)
{
	win->win_w = DEFAULT_WIDTH;
	win->win_h = DEFAULT_HEIGHT;
	win->mlx = mlx_init();
	if (!win->mlx)
		return ;
	win->wind = mlx_new_window(win->mlx, win->win_w,
			win->win_h + STATUS_HEIGHT, "so_long");
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
	make_window(&win);
	map.direction = DOWN;
	setting_key_hook(&win, &tex, &map, &st);
	free_objs(&map, map.map_height);
	return (0);
}
