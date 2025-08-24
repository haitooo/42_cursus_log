/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_sprite.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:28:40 by tssaito           #+#    #+#             */
/*   Updated: 2025/08/24 11:40:52 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Sorts the sprites from farthest to nearest relative to the player.
// This is necessary to ensure correct rendering order, so that sprites
// farther away are drawn before closer ones.
void	sort_sprites(t_game *game)
{
	t_sort_sprites_params	params;

	params.dist = malloc(sizeof(double) * game->num_sprites);
	if (!params.dist)
		exit_error("Malloc failed for sprite distances", game);
	calculate_sprite_distances(game, &params);
	sort_sprite_array(game, &params);
	free(params.dist);
}

// Calculates the rendering parameters for a single sprite.
// This includes its transformation relative to the camera,
//	its position on the screen,
// and its dimensions (height and width).
void	calculate_sprite_params(t_game *game, t_sprite *sprite,
		t_sprite_params *params)
{
	t_sprite_calc_params	calc_params;

	calculate_sprite_transform(game, sprite, &calc_params);
	params->transform_x = calc_params.transform_x;
	params->transform_y = calc_params.transform_y;
	calculate_sprite_screen_and_dimensions(game, &calc_params, params);
}

// Main function to draw all sprites on the screen.
// It first sorts the sprites, then iterates through them, calculates their
// rendering parameters, and draws each one vertical-strip by vertical-strip.
void	draw_sprites(t_game *game)
{
	int				i;
	int				stripe;
	t_sprite_params	params;

	sort_sprites(game);
	i = 0;
	while (i < game->num_sprites)
	{
		calculate_sprite_params(game, &game->sprites[i], &params);
		if (params.transform_y > 0)
		{
			stripe = params.draw_start_x;
			while (stripe < params.draw_end_x)
			{
				draw_sprite_vertical_strip(game, stripe, &params);
				stripe++;
			}
		}
		i++;
	}
}
