/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_sprite_sort_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:28:49 by tssaito           #+#    #+#             */
/*   Updated: 2025/08/24 11:40:59 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Swaps the values of two doubles.
void	double_swap(double *a, double *b)
{
	double	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

// Calculates the squared Euclidean distance from the player to each sprite.
// The squared distance is used for sorting, as it's computationally cheaper
// than the actual distance and sufficient for ordering.
void	calculate_sprite_distances(t_game *game, t_sort_sprites_params *params)
{
	params->i = 0;
	while (params->i < game->num_sprites)
	{
		params->dist[params->i] = ((game->player.pos_x
					- game->sprites[params->i].x) * (game->player.pos_x
					- game->sprites[params->i].x) + (game->player.pos_y
					- game->sprites[params->i].y) * (game->player.pos_y
					- game->sprites[params->i].y));
		params->i++;
	}
}

// Sorts the sprites based on their distance from the player,
//	from farthest to nearest.
// This uses a simple selection sort algorithm.
// It simultaneously sorts the distance array to maintain the correct order.
void	sort_sprite_array(t_game *game, t_sort_sprites_params *params)
{
	t_sprite	temp;

	params->i = 0;
	while (params->i < game->num_sprites - 1)
	{
		params->j = params->i + 1;
		while (params->j < game->num_sprites)
		{
			if (params->dist[params->i] < params->dist[params->j])
			{
				temp = game->sprites[params->i];
				game->sprites[params->i] = game->sprites[params->j];
				game->sprites[params->j] = temp;
				double_swap(&params->dist[params->i], &params->dist[params->j]);
			}
			params->j++;
		}
		params->i++;
	}
}
