/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:27:36 by tssaito           #+#    #+#             */
/*   Updated: 2025/08/24 11:40:11 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

#define COLLISION_MARGIN 0.2

// Checks if a sprite is located at the given map coordinates.
static int	is_sprite_at(t_game *game, int x, int y)
{
	int	i;

	i = 0;
	while (i < game->num_sprites)
	{
		if ((int)game->sprites[i].x == x && (int)game->sprites[i].y == y)
			return (1);
		i++;
	}
	return (0);
}

// Moves the player forward, checking for collisions with walls, doors,
//	and sprites.
void	move_forward(t_game *game)
{
	double	new_pos_x;
	double	new_pos_y;
	int		map_check_x;
	int		map_check_y;

	new_pos_x = game->player.pos_x + game->player.dir_x * MOVE_SPEED;
	new_pos_y = game->player.pos_y + game->player.dir_y * MOVE_SPEED;
	map_check_x = (int)(game->player.pos_x + game->player.dir_x * (MOVE_SPEED
				+ COLLISION_MARGIN));
	map_check_y = (int)(game->player.pos_y);
	if (game->map.grid[map_check_y][map_check_x] != '1'
		&& (game->map.grid[map_check_y][map_check_x] != 'D'
			|| is_door_open(game, map_check_x, map_check_y))
		&& !is_sprite_at(game, map_check_x, map_check_y))
		game->player.pos_x = new_pos_x;
	map_check_x = (int)(game->player.pos_x);
	map_check_y = (int)(game->player.pos_y + game->player.dir_y * (MOVE_SPEED
				+ COLLISION_MARGIN));
	if (game->map.grid[map_check_y][map_check_x] != '1'
		&& (game->map.grid[map_check_y][map_check_x] != 'D'
			|| is_door_open(game, map_check_x, map_check_y))
		&& !is_sprite_at(game, map_check_x, map_check_y))
		game->player.pos_y = new_pos_y;
}

// Moves the player backward, with collision checks.
void	move_backward(t_game *game)
{
	double	new_pos_x;
	double	new_pos_y;
	int		map_check_x;
	int		map_check_y;

	new_pos_x = game->player.pos_x - game->player.dir_x * MOVE_SPEED;
	new_pos_y = game->player.pos_y - game->player.dir_y * MOVE_SPEED;
	map_check_x = (int)(game->player.pos_x - game->player.dir_x * (MOVE_SPEED
				+ COLLISION_MARGIN));
	map_check_y = (int)(game->player.pos_y);
	if (game->map.grid[map_check_y][map_check_x] != '1'
		&& (game->map.grid[map_check_y][map_check_x] != 'D'
			|| is_door_open(game, map_check_x, map_check_y))
		&& !is_sprite_at(game, map_check_x, map_check_y))
		game->player.pos_x = new_pos_x;
	map_check_x = (int)(game->player.pos_x);
	map_check_y = (int)(game->player.pos_y - game->player.dir_y * (MOVE_SPEED
				+ COLLISION_MARGIN));
	if (game->map.grid[map_check_y][map_check_x] != '1'
		&& (game->map.grid[map_check_y][map_check_x] != 'D'
			|| is_door_open(game, map_check_x, map_check_y))
		&& !is_sprite_at(game, map_check_x, map_check_y))
		game->player.pos_y = new_pos_y;
}

// Moves the player left (strafe), with collision checks.
void	move_left(t_game *game)
{
	double	new_pos_x;
	double	new_pos_y;
	int		map_check_x;
	int		map_check_y;

	new_pos_x = game->player.pos_x - game->player.plane_x * MOVE_SPEED;
	new_pos_y = game->player.pos_y - game->player.plane_y * MOVE_SPEED;
	map_check_x = (int)(game->player.pos_x - game->player.plane_x * (MOVE_SPEED
				+ COLLISION_MARGIN));
	map_check_y = (int)(game->player.pos_y);
	if (game->map.grid[map_check_y][map_check_x] != '1'
		&& (game->map.grid[map_check_y][map_check_x] != 'D'
			|| is_door_open(game, map_check_x, map_check_y))
		&& !is_sprite_at(game, map_check_x, map_check_y))
		game->player.pos_x = new_pos_x;
	map_check_x = (int)(game->player.pos_x);
	map_check_y = (int)(game->player.pos_y - game->player.plane_y * (MOVE_SPEED
				+ COLLISION_MARGIN));
	if (game->map.grid[map_check_y][map_check_x] != '1'
		&& (game->map.grid[map_check_y][map_check_x] != 'D'
			|| is_door_open(game, map_check_x, map_check_y))
		&& !is_sprite_at(game, map_check_x, map_check_y))
		game->player.pos_y = new_pos_y;
}

// Moves the player right (strafe), with collision checks.
void	move_right(t_game *game)
{
	double	new_pos_x;
	double	new_pos_y;
	int		map_check_x;
	int		map_check_y;

	new_pos_x = game->player.pos_x + game->player.plane_x * MOVE_SPEED;
	new_pos_y = game->player.pos_y + game->player.plane_y * MOVE_SPEED;
	map_check_x = (int)(game->player.pos_x + game->player.plane_x * (MOVE_SPEED
				+ COLLISION_MARGIN));
	map_check_y = (int)(game->player.pos_y);
	if (game->map.grid[map_check_y][map_check_x] != '1'
		&& (game->map.grid[map_check_y][map_check_x] != 'D'
			|| is_door_open(game, map_check_x, map_check_y))
		&& !is_sprite_at(game, map_check_x, map_check_y))
		game->player.pos_x = new_pos_x;
	map_check_x = (int)(game->player.pos_x);
	map_check_y = (int)(game->player.pos_y + game->player.plane_y * (MOVE_SPEED
				+ COLLISION_MARGIN));
	if (game->map.grid[map_check_y][map_check_x] != '1'
		&& (game->map.grid[map_check_y][map_check_x] != 'D'
			|| is_door_open(game, map_check_x, map_check_y))
		&& !is_sprite_at(game, map_check_x, map_check_y))
		game->player.pos_y = new_pos_y;
}
