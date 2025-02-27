/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 14:15:39 by haito             #+#    #+#             */
/*   Updated: 2025/02/28 06:48:59 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	start_game(t_deta *deta)
{
	init_texture(deta->tex);
	import_texture(deta);
	if (!deta->win || !deta->win->wind || !deta->win->mlx)
		error_import(deta);
	if (mlx_hook(deta->win->wind, X_CLICK, 0, key_esc, deta) == 0)
		error_import(deta);
	if (mlx_loop_hook(deta->win->mlx, &draw_map, deta) == 0)
		error_import(deta);
	mlx_loop(deta->win->mlx);
}

void	setting_key_hook(t_window *win, t_texture *tex, t_map *map, t_st *st)
{
	t_deta	*deta;

	st->count_got_item = 0;
	st->count_step = 0;
	st->count_frame = 0;
	st->hp = 10;
	tex->img_width = IMG_WIDTH;
	tex->img_height = IMG_HEIGHT;
	tex->status_width = STATUS_WIDTH;
	tex->status_height = STATUS_HEIGHT;
	map->current_map_x = (map->x / NUM_OF_TILEX) * NUM_OF_TILEX;
	map->current_map_y = (map->y / NUM_OF_TILEY) * NUM_OF_TILEY;
	deta = malloc(sizeof(t_deta));
	if (!deta)
		error_hook(win, map, deta, ERRNO_MALLOC);
	deta->win = win;
	deta->tex = tex;
	deta->map = map;
	deta->st = st;
	if (mlx_key_hook(win->wind, (void *)call_keypress_func, deta) == 0)
		error_hook(win, map, deta, ERRNO_HOOK);
	start_game(deta);
}

void	make_window(t_window *win, t_map *map)
{
	win->win_w = DEFAULT_WIDTH;
	win->win_h = DEFAULT_HEIGHT;
	win->mlx = mlx_init();
	if (!win->mlx)
	{
		ft_dprintf("Error: %s\n", strerror(errno));
		free_objs(map, map->map_height);
		exit (1);
	}
	win->wind = mlx_new_window(win->mlx, win->win_w,
			win->win_h + STATUS_HEIGHT, "so_long");
	if (!win->wind)
	{
		if (win->mlx)
		{
			ft_dprintf("Error: %s\n", strerror(errno));
			free_objs(map, map->map_height);
			mlx_destroy_display(win->mlx);
			free(win->mlx);
			exit (1);
		}
	}
}

void	check_ber(const char *filename)
{
	int	length;

	length = ft_strlen(filename);
	if (length < 4)
	{
		ft_dprintf("Error: incorrect map name\n");
		exit(1);
	}
	if (ft_strcmp(filename + (length - 4), ".ber") != 0)
	{
		ft_dprintf("Error: incorrect map name\n");
		exit(1);
	}
}

int	main(int argc, char **argv)
{
	t_window	win;
	t_tex_num	n;
	t_texture	tex;
	t_map		map;
	t_st		st;

	if (argc != 2)
	{
		ft_dprintf("Error: incorrect argument\n");
		return (1);
	}
	tex.n = &n;
	init_objs_count(&map);
	check_ber(argv[1]);
	check_map_error(&map, argv[1]);
	put_enemy(&map);
	map.direction = DOWN;
	make_window(&win, &map);
	setting_key_hook(&win, &tex, &map, &st);
	free_objs(&map, map.map_height);
	return (0);
}
