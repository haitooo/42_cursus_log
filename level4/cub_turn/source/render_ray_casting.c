/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_ray_casting.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:28:38 by tssaito           #+#    #+#             */
/*   Updated: 2025/08/24 11:40:50 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Sets the initial side distance and step direction for the x-coordinate.
// This is based on the ray's direction to determine
// whether to step left or right
// and calculate the distance to the first x-side.
static void	set_side_dist_x(t_game *game, t_ray *ray)
{
	if (ray->ray_dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_dist_x = (game->player.pos_x - ray->map_x)
			* ray->delta_dist_x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist_x = (ray->map_x + 1.0 - game->player.pos_x)
			* ray->delta_dist_x;
	}
}

// Sets the initial side distance and step direction for the y-coordinate.
// Similar to set_side_dist_x, but for the vertical direction.
static void	set_side_dist_y(t_game *game, t_ray *ray)
{
	if (ray->ray_dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (game->player.pos_y - ray->map_y)
			* ray->delta_dist_y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist_y = (ray->map_y + 1.0 - game->player.pos_y)
			* ray->delta_dist_y;
	}
}

// Initializes the parameters for a single ray to be cast.
// This sets up the ray's direction, its position on the map, the distance
// to the next x and y grid lines (delta_dist), and the initial side distances.
void	calculate_ray_params(t_game *game, t_ray *ray, int x)
{
	ray->camera_x = 2 * x / (double)game->win_width - 1;
	ray->ray_dir_x = game->player.dir_x + game->player.plane_x * ray->camera_x;
	ray->ray_dir_y = game->player.dir_y + game->player.plane_y * ray->camera_x;
	ray->map_x = (int)game->player.pos_x;
	ray->map_y = (int)game->player.pos_y;
	ray->delta_dist_x = fabs(1.0 / ray->ray_dir_x);
	ray->delta_dist_y = fabs(1.0 / ray->ray_dir_y);
	ray->hit = 0;
	set_side_dist_x(game, ray);
	set_side_dist_y(game, ray);
}

// Checks if the current ray position has collided with a wall or a closed door.
// It sets the ray's hit flag to 1 if a collision is detected.
static void	check_ray_collision(t_game *game, t_ray *ray)
{
	if (ray->map_x < 0 || ray->map_x >= game->map.width || ray->map_y < 0
		|| ray->map_y >= game->map.height)
	{
		ray->hit = 1;
		return ;
	}
	if (game->map.grid[ray->map_y][ray->map_x] == '1')
		ray->hit = 1;
	else if (game->map.grid[ray->map_y][ray->map_x] == 'D')
	{
		if (is_door_open(game, ray->map_x, ray->map_y) == 0)
			ray->hit = 1;
	}
}

// Implements the Digital Differential Analysis (DDA) algorithm.
// It extends the ray from its current position to the next grid line in either
// the x or y direction, until a wall or closed door is hit.
void	perform_dda(t_game *game, t_ray *ray)
{
	while (ray->hit == 0)
	{
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->side_dist_x += ray->delta_dist_x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist_y += ray->delta_dist_y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		check_ray_collision(game, ray);
	}
}
