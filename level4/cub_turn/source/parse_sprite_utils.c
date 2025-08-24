/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_sprite_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:28:26 by tssaito           #+#    #+#             */
/*   Updated: 2025/08/24 11:40:40 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Initializes the data for a single sprite.
void	init_single_sprite_data(t_game *game, int i, int j, int *sprite_idx)
{
	game->sprites[*sprite_idx].x = j + 0.5;
	game->sprites[*sprite_idx].y = i + 0.5;
	(*sprite_idx)++;
}

// Initializes the data for all sprites found in the map.
void	init_sprite_data(t_game *game)
{
	int	i;
	int	j;
	int	sprite_idx;

	sprite_idx = 0;
	i = 0;
	while (i < game->map.height)
	{
		j = 0;
		while (j < game->map.width)
		{
			if (game->map.grid[i][j] == 'X')
			{
				init_single_sprite_data(game, i, j, &sprite_idx);
			}
			j++;
		}
		i++;
	}
}
