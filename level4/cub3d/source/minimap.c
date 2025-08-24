/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:26:58 by tssaito           #+#    #+#             */
/*   Updated: 2025/08/24 11:45:25 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Draws a single tile on the minimap representing a door,
//	colored based on its state.
static int	draw_minimap_door_tile(t_game *game, int x, int y)
{
	int	i;

	i = 0;
	while (i < game->num_doors)
	{
		if (game->doors[i].x == x && game->doors[i].y == y)
		{
			if (game->doors[i].is_open == 1 || game->doors[i].is_animating == 1)
				draw_square(game, x, y, MINIMAP_OPEN_DOOR_COLOR);
			else
				draw_square(game, x, y, MINIMAP_CLOSED_DOOR_COLOR);
			return (1);
		}
		i++;
	}
	return (0);
}

// Draws the static background of the minimap, including walls and floors.
static void	draw_minimap_background(t_game *game)
{
	int	x;
	int	y;

	y = 0;
	while (y < game->map.height)
	{
		x = 0;
		while (x < game->map.width)
		{
			if (!draw_minimap_door_tile(game, x, y))
			{
				if (game->map.grid[y][x] == '1')
					draw_square(game, x, y, MINIMAP_WALL_COLOR);
				else if (game->map.grid[y][x] == '0' || ft_strchr("NSEW",
						game->map.grid[y][x]))
					draw_square(game, x, y, MINIMAP_FLOOR_COLOR);
			}
			x++;
		}
		y++;
	}
}

// Draws the player's position and direction vector on the minimap.
static void	draw_player_and_direction(t_game *game, t_minimap *minimap)
{
	minimap->player_x = (int)(game->player.pos_x * MINIMAP_SCALE);
	minimap->player_y = (int)(game->player.pos_y * MINIMAP_SCALE);
	minimap->dir_line_x = minimap->player_x;
	minimap->dir_line_y = minimap->player_y;
	minimap->i = 0;
	while (minimap->i < MINIMAP_SCALE / 2)
	{
		put_pixel_to_img(&game->img, (int)minimap->dir_line_x,
			(int)minimap->dir_line_y, MINIMAP_DIR_COLOR);
		minimap->dir_line_x += game->player.dir_x;
		minimap->dir_line_y += game->player.dir_y;
		minimap->i++;
	}
	draw_player_circle(game, minimap->player_x, minimap->player_y, 2);
}

// Draws sprites on the minimap as star shapes.
static void	draw_sprites_on_minimap(t_game *game)
{
	int				i;
	t_star_params	star_params;

	i = 0;
	while (i < game->num_sprites)
	{
		star_params.center_x = (int)(game->sprites[i].x * MINIMAP_SCALE);
		star_params.center_y = (int)(game->sprites[i].y * MINIMAP_SCALE);
		star_params.size = 5;
		star_params.color = 0x0000FF00;
		draw_star(game, &star_params);
		i++;
	}
}

// Main function to draw the entire minimap.
void	draw_minimap(t_game *game)
{
	t_minimap	minimap;
	int			x;
	int			y;

	if (!game->show_minimap)
		return ;
	y = 0;
	while (y < game->map.height * MINIMAP_SCALE)
	{
		x = 0;
		while (x < game->map.width * MINIMAP_SCALE)
		{
			put_pixel_to_img(&game->img, x, y, MINIMAP_FLOOR_COLOR);
			x++;
		}
		y++;
	}
	draw_minimap_background(game);
	draw_player_and_direction(game, &minimap);
	draw_sprites_on_minimap(game);
}
