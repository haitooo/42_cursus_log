/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game_data.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:26:33 by tssaito           #+#    #+#             */
/*   Updated: 2025/08/24 12:10:25 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Initializes MLX and main image data.
void	init_game_mlx_and_img(t_game *game)
{
	game->mlx = NULL;
	game->win = NULL;
	game->win_width = WIN_WIDTH;
	game->win_height = WIN_HEIGHT;
	game->img.img_ptr = NULL;
	game->img.addr = NULL;
	game->img.bits_per_pixel = 0;
	game->img.line_length = 0;
	game->img.endian = 0;
}

// Initializes player-related data like position and direction.
void	init_game_player_data(t_game *game)
{
	game->player.pos_x = 0;
	game->player.pos_y = 0;
	game->player.dir_x = 0;
	game->player.dir_y = 0;
	game->player.plane_x = 0;
	game->player.plane_y = 0;
}

// Initializes map data, setting the grid to NULL and dimensions to 0.
void	init_game_map_data(t_game *game)
{
	game->map.grid = NULL;
	game->map.width = 0;
	game->map.height = 0;
}

// Initializes game configuration data, including textures, colors,
// and entities.
void	init_game_config_data(t_game *game)
{
	game->config.north_texture_path = NULL;
	game->config.south_texture_path = NULL;
	game->config.west_texture_path = NULL;
	game->config.east_texture_path = NULL;
	game->config.door_texture_path = NULL;
	game->config.animated_sprite_texture_paths = NULL;
	game->config.num_animated_sprite_frames = 0;
	game->config.animated_sprite_paths_capacity = 0;
	game->config.floor_color = -1;
	game->config.ceiling_color = -1;
	game->textures = NULL;
	game->num_textures = 0;
	game->doors = NULL;
	game->num_doors = 0;
	game->sprites = NULL;
	game->num_sprites = 0;
	game->first_mouse_move = 1;
	game->z_buffer = NULL;
	game->frame_count = 0;
	game->show_minimap = 1;
}
