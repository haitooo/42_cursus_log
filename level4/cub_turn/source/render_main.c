/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:28:31 by tssaito           #+#    #+#             */
/*   Updated: 2025/08/24 11:40:45 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Clears the background by filling the top half of the screen
// with the ceiling color
// and the bottom half with the floor color.
void	clear_background(t_game *game)
{
	int	x;
	int	y;

	y = 0;
	while (y < game->win_height)
	{
		x = 0;
		while (x < game->win_width)
		{
			if (y < game->win_height / 2)
				my_mlx_pixel_put(&game->img, x, y, game->config.ceiling_color);
			else
				my_mlx_pixel_put(&game->img, x, y, game->config.floor_color);
			x++;
		}
		y++;
	}
}

// Checks if the ray has hit a door and sets the open_progress for rendering.
// If the ray's map coordinates match a door's coordinates, it updates the
// progress; otherwise, it's set to a default (fully open).
void	init_ray_render_params_door_check(t_game *game,
		t_ray_render_params *params)
{
	int	i;

	params->strip_params.open_progress = 1.0;
	i = 0;
	while (i < game->num_doors)
	{
		if (params->ray.map_x == game->doors[i].x
			&& params->ray.map_y == game->doors[i].y)
		{
			params->strip_params.open_progress = game->doors[i].open_progress;
			break ;
		}
		i++;
	}
}

// Processes the rendering for a single vertical strip (column) of the screen.
// This involves casting a ray, calculating wall distance and height,
// determining the correct texture and texture coordinates,
//	and drawing the strip.
void	process_wall_rendering_loop_details(t_game *game,
		t_ray_render_params *params, int x)
{
	calculate_ray_params(game, &params->ray, x);
	perform_dda(game, &params->ray);
	calculate_wall_projection(game, &params->ray, &params->proj);
	get_texture_params(game, &params->ray, &params->tex_info);
	params->strip_params.x = x;
	params->strip_params.line_height = params->proj.line_height;
	params->strip_params.draw_start = params->proj.draw_start;
	params->strip_params.draw_end = params->proj.draw_end;
	params->strip_params.tex_num = params->tex_info.tex_num;
	params->strip_params.tex_x = params->tex_info.tex_x;
	init_ray_render_params_door_check(game, params);
	draw_vertical_strip(game, &params->strip_params);
	game->z_buffer[x] = params->ray.perp_wall_dist;
}

// Iterates through all doors and updates their animation state if they are
// currently opening or closing.
void	update_door_animation(t_game *game)
{
	int	i;

	i = 0;
	while (i < game->num_doors)
	{
		if (game->doors[i].is_animating)
		{
			update_single_door_animation(game, i);
		}
		i++;
	}
}

// Main rendering function that is called for each frame.
// It orchestrates the entire rendering process: clearing the screen,
// casting rays for walls, drawing sprites, drawing the minimap,
// and updating animations.
int	render_frame(t_game *game)
{
	int					x;
	t_ray_render_params	params;

	clear_background(game);
	if (!game->z_buffer)
		game->z_buffer = malloc(sizeof(double) * game->win_width);
	if (!game->z_buffer)
		exit_error("Malloc failed for z_buffer", game);
	x = 0;
	while (x < game->win_width)
	{
		process_wall_rendering_loop_details(game, &params, x);
		x++;
	}
	if (game->num_sprites > 0)
		draw_sprites(game);
	draw_minimap(game);
	mlx_put_image_to_window(game->mlx, game->win, game->img.img_ptr, 0, 0);
	game->frame_count++;
	check_and_close_doors(game);
	update_door_animation(game);
	return (0);
}
