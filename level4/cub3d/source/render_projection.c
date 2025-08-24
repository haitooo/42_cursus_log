/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_projection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:28:35 by tssaito           #+#    #+#             */
/*   Updated: 2025/08/24 11:40:48 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Calculates the vertical drawing range for a wall slice.
// It determines the top and bottom pixel coordinates (draw_start, draw_end)
// for the wall strip based on its projected height, ensuring it stays within
// the screen boundaries.
static void	calculate_draw_range(t_game *game, t_wall_projection *proj)
{
	proj->draw_start = -(proj->line_height) / 2 + game->win_height / 2;
	if (proj->draw_start < 0)
		proj->draw_start = 0;
	proj->draw_end = (proj->line_height) / 2 + game->win_height / 2;
	if (proj->draw_end >= game->win_height)
		proj->draw_end = game->win_height - 1;
}

// Calculates the necessary parameters for wall projection.
// This includes the perpendicular distance to the wall
// to prevent fisheye distortion,
// the height of the wall line to be drawn on screen, and the start and end
// points for drawing.
void	calculate_wall_projection(t_game *game, t_ray *ray,
		t_wall_projection *proj)
{
	if (ray->side == 0)
		ray->perp_wall_dist = (ray->map_x - game->player.pos_x + (1
					- ray->step_x) / 2) / ray->ray_dir_x;
	else
		ray->perp_wall_dist = (ray->map_y - game->player.pos_y + (1
					- ray->step_y) / 2) / ray->ray_dir_y;
	if (ray->perp_wall_dist <= 0)
		ray->perp_wall_dist = 0.001;
	proj->line_height = (int)(game->win_height / ray->perp_wall_dist);
	calculate_draw_range(game, proj);
}
