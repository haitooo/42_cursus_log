/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map_validation.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:28:06 by tssaito           #+#    #+#             */
/*   Updated: 2025/08/24 12:44:02 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	validate_texture_requirements(t_game *game)
{
	bool	has_x;
	bool	has_d;
	int		y;
	int		x;

	has_x = false;
	has_d = false;
	y = 0;
	while (y < game->map.height)
	{
		x = 0;
		while (x < game->map.width)
		{
			if (game->map.grid[y][x] == 'X')
				has_x = true;
			if (game->map.grid[y][x] == 'D')
				has_d = true;
			x++;
		}
		y++;
	}
	if (has_x && game->config.animated_sprite_texture_paths == NULL)
		exit_error("Map contains 'X' but no X1 texture is defined", game);
	if (has_d && game->config.door_texture_path == NULL)
		exit_error("Map contains 'D' but no DO texture is defined", game);
}

// Processes a single character in the map, initializing the player if found.
void	process_map_character(t_game *game, int y, int x, int *player_count)
{
	if (ft_strchr("01NSEWDX ", game->map.grid[y][x]) == NULL)
		exit_error("Invalid character in map", game);
	if (ft_strchr("NSEW", game->map.grid[y][x]) != NULL)
	{
		init_player(game, game->map.grid[y][x], x, y);
		game->map.grid[y][x] = '0';
		(*player_count)++;
	}
}

// Checks if a specific position on the map is correctly surrounded by walls.
void	check_wall_at_position(t_game *game, int y, int x)
{
	if (game->map.grid[y][x] == '0' || game->map.grid[y][x] == 'D')
	{
		if (y == 0 || y == game->map.height - 1 || x == 0
			|| x == game->map.width - 1)
			exit_error("Map not closed by walls", game);
		if ((y > 0 && game->map.grid[y - 1][x] == ' ') || (y < game->map.height
				- 1 && game->map.grid[y + 1][x] == ' ') || (x > 0
				&& game->map.grid[y][x - 1] == ' ') || (x < game->map.width - 1
				&& game->map.grid[y][x + 1] == ' '))
			exit_error("Map not closed by walls", game);
	}
}

// Main function to validate the entire map.
void	validate_map(t_game *game)
{
	int	player_count;

	player_count = 0;
	validate_map_characters(game, &player_count);
	check_map_walls(game);
	validate_texture_requirements(game);
	validate_animated_sprite_sequence(game);
	process_doors(game);
	process_sprites(game);
}

void	validate_animated_sprite_sequence(t_game *game)
{
	int	i;

	if (game->config.num_animated_sprite_frames > 0)
	{
		i = 0;
		while (i < game->config.num_animated_sprite_frames)
		{
			if (game->config.animated_sprite_texture_paths[i] == NULL)
				exit_error("Animated sprite texture sequence is not continuous",
					game);
			i++;
		}
	}
}
