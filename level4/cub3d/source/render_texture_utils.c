/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_texture_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:28:55 by tssaito           #+#    #+#             */
/*   Updated: 2025/08/24 11:41:45 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Retrieves the color of a single pixel from the specified texture.
// It calculates the memory offset for the given texture coordinates
// (tex_x,tex_y)
// and returns the color value at that position.
unsigned int	get_texture_pixel_color(t_game *game, t_strip_params *params,
		int tex_y)
{
	return (*(unsigned int *)(game->textures[params->tex_num].addr + (tex_y
			* game->textures[params->tex_num].line_length + params->tex_x
			* (game->textures[params->tex_num].bits_per_pixel / 8))));
}

// Calculates the initial vertical texture position
// and the step size for traversing the texture.
// This is used to map the texture correctly onto the wall strip,
//	ensuring it scales properly with the wall's height on screen.
void	calculate_strip_texture_position(t_game *game, t_strip_params *params,
		t_vertical_strip_calc_params *calc_params)
{
	calc_params->step = 1.0 * game->textures[params->tex_num].height
		/ params->line_height;
	calc_params->tex_pos = (params->draw_start - game->win_height / 2
			+ params->line_height / 2) * calc_params->step;
}
