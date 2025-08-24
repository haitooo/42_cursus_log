/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_texture.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:28:52 by tssaito           #+#    #+#             */
/*   Updated: 2025/08/24 11:41:01 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Determines the appropriate texture number
// based on the wall side hit by the ray.
// It checks if the hit object is a door and assigns the door texture, otherwise
// it assigns the correct wall texture (North, South, East, West) based on the
// ray's direction and which side (x or y) was hit.
void	set_texture_number(t_game *game, t_ray *ray, t_texture_info *tex_info)
{
	char	hit_char;

	hit_char = game->map.grid[ray->map_y][ray->map_x];
	if (hit_char == 'D')
	{
		tex_info->tex_num = 4;
		return ;
	}
	if (ray->side == 0)
	{
		if (ray->ray_dir_x > 0)
			tex_info->tex_num = 3;
		else
			tex_info->tex_num = 2;
	}
	else
	{
		if (ray->ray_dir_y > 0)
			tex_info->tex_num = 1;
		else
			tex_info->tex_num = 0;
	}
}

// Adjusts the texture's x-coordinate to correct for mirroring.
// Depending on the ray's direction, the texture might be rendered mirrored.
// This function flips the x-coordinate to ensure the texture appears correctly.
void	adjust_texture_x(t_game *game, t_ray *ray, t_texture_info *tex_info)
{
	if (ray->side == 0 && ray->ray_dir_x > 0)
		tex_info->tex_x = game->textures[tex_info->tex_num].width
			- tex_info->tex_x - 1;
	if (ray->side == 1 && ray->ray_dir_y < 0)
		tex_info->tex_x = game->textures[tex_info->tex_num].width
			- tex_info->tex_x - 1;
}

// Calculates the necessary parameters for texturing a wall strip.
// It determines the exact point of impact on the wall (wall_x), selects the
// correct texture, calculates the horizontal texture coordinate (tex_x), and
// adjusts it to prevent mirroring.
void	get_texture_params(t_game *game, t_ray *ray, t_texture_info *tex_info)
{
	if (ray->side == 0)
		tex_info->wall_x = game->player.pos_y + ray->perp_wall_dist
			* ray->ray_dir_y;
	else
		tex_info->wall_x = game->player.pos_x + ray->perp_wall_dist
			* ray->ray_dir_x;
	tex_info->wall_x -= floor(tex_info->wall_x);
	set_texture_number(game, ray, tex_info);
	tex_info->tex_x = (int)(tex_info->wall_x
			* game->textures[tex_info->tex_num].width);
	if (tex_info->tex_num == 0 || tex_info->tex_num == 1
		|| tex_info->tex_num == 2 || tex_info->tex_num == 3
		|| tex_info->tex_num == 4)
		tex_info->tex_x = game->textures[tex_info->tex_num].width
			- tex_info->tex_x - 1;
	adjust_texture_x(game, ray, tex_info);
}

// Adjusts the texture x-coordinate for doors to create an opening effect.
// It shifts the texture based on the door's open_progress, making it appear
// as if the door is sliding open.
void	adjust_door_texture_x(t_game *game, t_strip_params *params)
{
	if (params->tex_num == 4)
		params->tex_x = (int)(params->tex_x - params->open_progress
				* game->textures[params->tex_num].width);
	if (params->tex_x < 0)
		params->tex_x = 0;
	if (params->tex_x >= game->textures[params->tex_num].width)
		params->tex_x = game->textures[params->tex_num].width - 1;
}

// Draws a single vertical strip of a textured wall or door.
// It iterates from the top to the bottom of the wall slice on screen,
// calculating the corresponding vertical texture coordinate for each pixel,
// fetching the color, and drawing it to the image buffer.
void	draw_vertical_strip(t_game *game, t_strip_params *params)
{
	int								y;
	int								tex_y;
	unsigned int					color;
	t_vertical_strip_calc_params	calc_params;

	calculate_strip_texture_position(game, params, &calc_params);
	y = params->draw_start;
	while (y < params->draw_end)
	{
		tex_y = (int)calc_params.tex_pos;
		if (tex_y < 0)
			tex_y = 0;
		if (tex_y >= game->textures[params->tex_num].height)
			tex_y = game->textures[params->tex_num].height - 1;
		calc_params.tex_pos += calc_params.step;
		adjust_door_texture_x(game, params);
		color = get_texture_pixel_color(game, params, tex_y);
		my_mlx_pixel_put(&game->img, params->x, y, color);
		y++;
	}
}
