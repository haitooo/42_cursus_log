/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:26:30 by tssaito           #+#    #+#             */
/*   Updated: 2025/08/24 11:39:52 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Initializes all core game data structures.
void	init_game(t_game *game)
{
	init_game_mlx_and_img(game);
	init_game_player_data(game);
	init_game_map_data(game);
	init_game_config_data(game);
}

// Initializes the player's starting position and orientation.
void	init_player(t_game *game, char orientation, int x, int y)
{
	game->player.pos_x = x + 0.5;
	game->player.pos_y = y + 0.5;
	if (orientation == 'N')
		set_player_direction_north(&game->player);
	else if (orientation == 'S')
		set_player_direction_south(&game->player);
	else if (orientation == 'E')
		set_player_direction_east(&game->player);
	else if (orientation == 'W')
		set_player_direction_west(&game->player);
}
