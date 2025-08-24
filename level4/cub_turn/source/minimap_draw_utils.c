/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_draw_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:27:29 by tssaito           #+#    #+#             */
/*   Updated: 2025/08/24 11:40:05 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Puts a single pixel of a given color onto the image buffer
// at specified coordinates.
void	put_pixel_to_img(t_img *img, int x, int y, int color)
{
	char	*dst;

	if (x >= 0 && x < WIN_WIDTH && y >= 0 && y < WIN_HEIGHT)
	{
		dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel
					/ 8));
		*(unsigned int *)dst = color;
	}
}

// Draws a square on the minimap at a given map coordinate with a specific color
void	draw_square(t_game *game, int map_x, int map_y, int color)
{
	int	pixel_x;
	int	pixel_y;
	int	start_x;
	int	start_y;

	start_x = map_x * MINIMAP_SCALE;
	start_y = map_y * MINIMAP_SCALE;
	pixel_y = 0;
	while (pixel_y < MINIMAP_SCALE)
	{
		pixel_x = 0;
		while (pixel_x < MINIMAP_SCALE)
		{
			put_pixel_to_img(&game->img, start_x + pixel_x, start_y + pixel_y,
				color);
			pixel_x++;
		}
		pixel_y++;
	}
}

// Draws a circle on the minimap to represent the player.
void	draw_player_circle(t_game *game, int center_x, int center_y, int radius)
{
	int	x;
	int	y;

	y = -radius;
	while (y <= radius)
	{
		x = -radius;
		while (x <= radius)
		{
			if (x * x + y * y <= radius * radius)
				put_pixel_to_img(&game->img, center_x + x, center_y + y,
					MINIMAP_PLAYER_COLOR);
			x++;
		}
		y++;
	}
}

// Draws a star shape, used for representing sprites on the minimap.
void	draw_star(t_game *game, t_star_params *params)
{
	double	angle;
	double	x;
	double	y;
	int		i;
	int		j;

	i = 0;
	while (i < 5)
	{
		angle = i * 2.0 * M_PI / 5.0 - M_PI / 2.0;
		j = 0;
		while (j < params->size)
		{
			x = params->center_x + cos(angle) * j;
			y = params->center_y + sin(angle) * j;
			put_pixel_to_img(&game->img, (int)x, (int)y, params->color);
			j++;
		}
		i++;
	}
}
