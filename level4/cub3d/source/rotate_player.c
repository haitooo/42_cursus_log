/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate_player.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:29:25 by tssaito           #+#    #+#             */
/*   Updated: 2025/08/24 11:41:09 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Rotates the player's view to the left by applying a negative rotation speed.
// This function updates both the player's direction vector and the camera plane
void	rotate_left(t_game *game)
{
	rotate_player_direction(game, -ROT_SPEED);
	rotate_player_plane(game, -ROT_SPEED);
}

// Rotates the player's view to the right by applying a positive rotation speed.
// This function updates both the player's direction vector and the camera plane
void	rotate_right(t_game *game)
{
	rotate_player_direction(game, ROT_SPEED);
	rotate_player_plane(game, ROT_SPEED);
}

// Rotates the player's direction vector by a given angle (rot_speed).
// It uses a 2D rotation matrix to calculate the new direction coordinates.
void	rotate_player_direction(t_game *game, double rot_speed)
{
	double	old_dir_x;

	old_dir_x = game->player.dir_x;
	game->player.dir_x = game->player.dir_x * cos(rot_speed)
		- game->player.dir_y * sin(rot_speed);
	game->player.dir_y = old_dir_x * sin(rot_speed) + game->player.dir_y
		* cos(rot_speed);
}

// Rotates the camera plane vector by a given angle (rot_speed).
// The camera plane must be rotated in sync with the player's direction
// to maintain the correct field of view.
void	rotate_player_plane(t_game *game, double rot_speed)
{
	double	old_plane_x;

	old_plane_x = game->player.plane_x;
	game->player.plane_x = game->player.plane_x * cos(rot_speed)
		- game->player.plane_y * sin(rot_speed);
	game->player.plane_y = old_plane_x * sin(rot_speed) + game->player.plane_y
		* cos(rot_speed);
}
