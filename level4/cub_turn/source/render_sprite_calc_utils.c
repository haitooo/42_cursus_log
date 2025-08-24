/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_sprite_calc_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:28:43 by tssaito           #+#    #+#             */
/*   Updated: 2025/08/24 12:10:08 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// This function transforms sprite coordinates from world space to camera space.
// It calculates the sprite's position relative to the player and then applies
// a transformation based on the player's direction and camera plane.
void	calculate_sprite_transform(t_game *game, t_sprite *sprite,
		t_sprite_calc_params *calc_params)
{
	calc_params->sprite_x = sprite->x - game->player.pos_x;
	calc_params->sprite_y = sprite->y - game->player.pos_y;
	calc_params->inv_det = 1.0 / (game->player.plane_x * game->player.dir_y
			- game->player.dir_x * game->player.plane_y);
	calc_params->transform_x = calc_params->inv_det * (game->player.dir_y
			* calc_params->sprite_x - game->player.dir_x
			* calc_params->sprite_y);
	calc_params->transform_y = calc_params->inv_det * (-game->player.plane_y
			* calc_params->sprite_x + game->player.plane_x
			* calc_params->sprite_y);
}

// This function calculates the sprite's dimensions and drawing boundaries
// on the screen.
// It determines the sprite's height and width, as well as the start and end
// coordinates for drawing, ensuring they are clamped within the window bounds.
void	calculate_sprite_screen_and_dimensions(t_game *game,
		t_sprite_calc_params *calc_params, t_sprite_params *params)
{
	params->sprite_screen_x = (int)((game->win_width / 2) * (1
				+ calc_params->transform_x / calc_params->transform_y));
	params->sprite_height = abs((int)(game->win_height
				/ (calc_params->transform_y)));
	params->draw_start_y = -(params->sprite_height) / 2 + game->win_height / 2;
	if (params->draw_start_y < 0)
		params->draw_start_y = 0;
	params->draw_end_y = params->sprite_height / 2 + game->win_height / 2;
	if (params->draw_end_y >= game->win_height)
		params->draw_end_y = game->win_height - 1;
	params->sprite_width = abs((int)(game->win_height
				/ (calc_params->transform_y)));
	params->draw_start_x = -(params->sprite_width) / 2
		+ params->sprite_screen_x;
	if (params->draw_start_x < 0)
		params->draw_start_x = 0;
	params->draw_end_x = params->sprite_width / 2 + params->sprite_screen_x;
	if (params->draw_end_x >= game->win_width)
		params->draw_end_x = game->win_width - 1;
}

// This function calculates the texture coordinates
// for a vertical stripe of a sprite.
// It determines which frame of the animated sprite to use
// based on the game's frame count
// and calculates the corresponding x-coordinate on the texture.
void	calculate_sprite_tex_coords(t_game *game, t_tex_coord_calc_info *info,
		t_sprite_params *params)
{
	if (game->config.num_animated_sprite_frames > 0)
		*info->current_frame_tex_num = 5 + ((game->frame_count / 5)
				% game->config.num_animated_sprite_frames);
	else
		*info->current_frame_tex_num = 5;
	*info->tex_x = (int)(256 * (info->stripe - (-params->sprite_width / 2
					+ params->sprite_screen_x))
			* game->textures[*info->current_frame_tex_num].width
			/ params->sprite_width) / 256;
}
