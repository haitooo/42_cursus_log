/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_sprite_draw_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:28:46 by tssaito           #+#    #+#             */
/*   Updated: 2025/08/24 11:40:56 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Draws a single pixel of a sprite texture to the screen buffer.
// It retrieves the color from the sprite's texture data at the specified
// texture coordinates (tex_x, tex_y) and puts it on the image buffer
// at the screen coordinates (stripe, y). It skips transparent pixels.
void	draw_sprite_pixel(t_game *game, t_pixel_draw_info *info)
{
	unsigned int	color;

	color = *(unsigned int *)(game->textures[info->current_frame_tex_num].addr
			+ (info->tex_y
				* game->textures[info->current_frame_tex_num].line_length
				+ info->tex_x
				* (game->textures[info->current_frame_tex_num].bits_per_pixel
					/ 8)));
	if ((color & 0x00FFFFFF) != 0)
		my_mlx_pixel_put(&game->img, info->stripe, info->y, color);
}

// This function is the core loop for drawing a vertical strip of a sprite.
// It calculates the vertical texture coordinate (tex_y) for each pixel
// in the strip and then calls draw_sprite_pixel to draw it.
void	draw_sprite_strip_loop_body(t_game *game, int stripe,
		t_sprite_params *params, t_sprite_strip_vars *vars)
{
	t_pixel_draw_info	pixel_info;

	vars->d = (vars->y) * 256 - game->win_height * 128 + params->sprite_height
		* 128;
	vars->tex_y = ((vars->d
				* game->textures[vars->current_frame_tex_num].height)
			/ params->sprite_height) / 256;
	pixel_info.stripe = stripe;
	pixel_info.y = vars->y;
	pixel_info.tex_x = vars->tex_x;
	pixel_info.tex_y = vars->tex_y;
	pixel_info.current_frame_tex_num = vars->current_frame_tex_num;
	draw_sprite_pixel(game, &pixel_info);
}

// Draws a single vertical strip (column) of a sprite.
// It first calculates the horizontal texture coordinate (tex_x) and then loops
// through each vertical pixel of the strip, drawing it to the screen if it's
// in front of a wall (checked via the z_buffer).
void	draw_sprite_vertical_strip(t_game *game, int stripe,
		t_sprite_params *params)
{
	t_sprite_strip_vars		vars;
	t_tex_coord_calc_info	tex_coord_calc_info;

	tex_coord_calc_info.stripe = stripe;
	tex_coord_calc_info.tex_x = &vars.tex_x;
	tex_coord_calc_info.current_frame_tex_num = &vars.current_frame_tex_num;
	calculate_sprite_tex_coords(game, &tex_coord_calc_info, params);
	if (params->transform_y > 0 && stripe > 0 && stripe < game->win_width
		&& params->transform_y < game->z_buffer[stripe])
	{
		vars.y = params->draw_start_y;
		while (vars.y < params->draw_end_y)
		{
			draw_sprite_strip_loop_body(game, stripe, params, &vars);
			vars.y++;
		}
	}
}
