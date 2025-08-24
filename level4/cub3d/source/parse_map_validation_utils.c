/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map_validation_utils.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:28:11 by tssaito           #+#    #+#             */
/*   Updated: 2025/08/24 12:42:58 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Iterates through the map to process each character,
//	counting players and initializing them.
void	iterate_map_and_process_characters(t_game *game, int *player_count)
{
	int	x;
	int	y;

	y = -1;
	while (++y < game->map.height)
	{
		if (!game->map.grid[y])
			continue ;
		x = -1;
		while (++x < game->map.width)
		{
			process_map_character(game, y, x, player_count);
		}
	}
}

// Iterates through the map to check
// if all walkable areas are surrounded by walls.
void	iterate_map_and_check_walls(t_game *game)
{
	int	x;
	int	y;

	y = -1;
	while (++y < game->map.height)
	{
		if (!game->map.grid[y])
		{
			continue ;
		}
		x = -1;
		while (++x < game->map.width)
		{
			check_wall_at_position(game, y, x);
		}
	}
}

// Validates all characters in the map and ensures there is exactly one player.
void	validate_map_characters(t_game *game, int *player_count)
{
	iterate_map_and_process_characters(game, player_count);
	if (*player_count != 1)
		exit_error("Map must contain exactly one player start position", game);
}

// Checks if the entire map is correctly enclosed by walls.
void	check_map_walls(t_game *game)
{
	iterate_map_and_check_walls(game);
}
