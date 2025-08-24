/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_entities.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:27:50 by tssaito           #+#    #+#             */
/*   Updated: 2025/08/24 11:40:26 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Counts the number of doors ('D') in the game map.
int	count_doors_in_map(t_game *game)
{
	int	i;
	int	j;
	int	count;

	count = 0;
	i = 0;
	while (i < game->map.height)
	{
		j = 0;
		while (j < game->map.width)
		{
			if (game->map.grid[i][j] == 'D')
				count++;
			j++;
		}
		i++;
	}
	return (count);
}

// Initializes the data for all doors found in the map.
void	init_door_data(t_game *game)
{
	int	i;
	int	j;
	int	door_idx;

	door_idx = 0;
	i = 0;
	while (i < game->map.height)
	{
		j = 0;
		while (j < game->map.width)
		{
			if (game->map.grid[i][j] == 'D')
			{
				init_single_door_data(game, i, j, &door_idx);
			}
			j++;
		}
		i++;
	}
}

// Processes all doors in the map, counting and initializing them.
void	process_doors(t_game *game)
{
	game->num_doors = count_doors_in_map(game);
	if (game->num_doors == 0)
		return ;
	game->doors = malloc(sizeof(t_door) * game->num_doors);
	if (!game->doors)
		exit_error("Malloc failed for doors", game);
	init_door_data(game);
}

// Counts the number of sprites ('X') in the game map.
int	count_sprites_in_map(t_game *game)
{
	int	i;
	int	j;
	int	count;

	count = 0;
	i = 0;
	while (i < game->map.height)
	{
		j = 0;
		while (j < game->map.width)
		{
			if (game->map.grid[i][j] == 'X')
				count++;
			j++;
		}
		i++;
	}
	return (count);
}

// Processes all sprites in the map, counting and initializing them.
void	process_sprites(t_game *game)
{
	game->num_sprites = 0;
	game->num_sprites = count_sprites_in_map(game);
	if (game->num_sprites == 0)
		return ;
	game->sprites = malloc(sizeof(t_sprite) * game->num_sprites);
	if (!game->sprites)
		exit_error("Malloc failed for sprites", game);
	init_sprite_data(game);
}
