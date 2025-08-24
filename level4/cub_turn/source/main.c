/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:26:55 by tssaito           #+#    #+#             */
/*   Updated: 2025/08/24 17:55:17 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Sets up the MiniLibX environment, loads textures, and registers event hooks.
static void	setup_mlx_and_hooks(t_game *game)
{
	game->mlx = mlx_init();
	if (!game->mlx)
		exit_error("Could not initialize MiniLibX", game);
	load_textures(game);
	game->win = mlx_new_window(game->mlx, game->win_width, game->win_height,
			"cub3D");
	if (!game->win)
		exit_error("Could not create window", game);
	game->img.img_ptr = mlx_new_image(game->mlx, game->win_width,
			game->win_height);
	if (!game->img.img_ptr)
		exit_error("Could not create image", game);
	game->img.addr = mlx_get_data_addr(game->img.img_ptr,
			&game->img.bits_per_pixel, &game->img.line_length,
			&game->img.endian);
	mlx_hook(game->win, 2, 1, (int (*)())key_press_wrapper, game);
	mlx_hook(game->win, 17, 1L << 17, (int (*)())close_window_wrapper, game);
	mlx_hook(game->win, 6, 1L << 6, (int (*)())mouse_move_hook, game);
	mlx_loop_hook(game->mlx, (int (*)())render_frame_wrapper, game);
	mlx_mouse_hide(game->mlx, game->win);
}

// The main entry point of the program.
int	main(int argc, char **argv)
{
	t_game	*game;

	if (argc != 2)
		exit_error("Usage: ./cub3D <map_file.cub>", NULL);
	check_filename(argv[1]);
	game = malloc(sizeof(t_game));
	if (!game)
		exit_error("Could not allocate memory for game", NULL);
	init_game(game);
	parse_cub_file(argv[1], game);
	setup_mlx_and_hooks(game);
	mlx_loop(game->mlx);
	return (0);
}
