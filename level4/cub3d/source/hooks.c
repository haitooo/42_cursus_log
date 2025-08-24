/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:26:26 by tssaito           #+#    #+#             */
/*   Updated: 2025/08/24 11:39:50 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Handles key press events for player movement, rotation, and interaction.
int	key_press_hook(int keycode, t_game *game)
{
	if (keycode == KEY_W || keycode == KEY_Z)
		move_forward(game);
	else if (keycode == KEY_S)
		move_backward(game);
	else if (keycode == KEY_A || keycode == KEY_Q)
		move_left(game);
	else if (keycode == KEY_D)
		move_right(game);
	else if (keycode == KEY_LEFT)
		rotate_left(game);
	else if (keycode == KEY_RIGHT)
		rotate_right(game);
	else if (keycode == KEY_ESC)
		close_window_hook(game);
	else if (keycode == KEY_E)
		handle_door_interaction(game);
	else if (keycode == KEY_M)
		game->show_minimap = !game->show_minimap;
	return (0);
}

// Handles mouse movement for player rotation.
int	mouse_move_hook(int x, int y, t_game *game)
{
	int		center_x;
	int		center_y;
	double	rot_speed;

	center_x = game->win_width / 2;
	center_y = game->win_height / 2;
	if (game->first_mouse_move)
	{
		game->first_mouse_move = 0;
		return (0);
	}
	if (x == center_x && y == center_y)
		return (0);
	if (x != center_x)
	{
		rot_speed = (x - center_x) * MOUSE_SENSITIVITY;
		rotate_player_direction(game, rot_speed);
		rotate_player_plane(game, rot_speed);
	}
	mlx_mouse_move(game->mlx, game->win, center_x, center_y);
	return (0);
}

// Handles the window close event, freeing resources and exiting.
int	close_window_hook(t_game *game)
{
	mlx_loop_end(game->mlx);
	free_game(game);
	exit(EXIT_SUCCESS);
}
