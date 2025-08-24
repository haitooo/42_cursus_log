/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_textures.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:26:39 by tssaito           #+#    #+#             */
/*   Updated: 2025/08/24 12:12:01 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Loads a single texture from a given path into the game's texture data.
void	load_single_texture(t_game *game, t_img *texture, char *path)
{
	texture->img_ptr = mlx_xpm_file_to_image(game->mlx, path, &texture->width,
			&texture->height);
	if (!texture->img_ptr)
		exit_error("Failed to load texture", game);
	texture->addr = mlx_get_data_addr(texture->img_ptr,
			&texture->bits_per_pixel, &texture->line_length, &texture->endian);
	if (!texture->addr)
		exit_error("Failed to get texture address", game);
}

static void	load_static_textures(t_game *game)
{
	load_single_texture(game, &game->textures[0],
		game->config.north_texture_path);
	load_single_texture(game, &game->textures[1],
		game->config.south_texture_path);
	load_single_texture(game, &game->textures[2],
		game->config.west_texture_path);
	load_single_texture(game, &game->textures[3],
		game->config.east_texture_path);
	if (game->config.door_texture_path)
		load_single_texture(game, &game->textures[4],
			game->config.door_texture_path);
}

static void	load_animated_textures(t_game *game)
{
	int	i;

	i = 0;
	while (i < game->config.num_animated_sprite_frames)
	{
		if (game->config.animated_sprite_texture_paths[i])
			load_single_texture(game, &game->textures[5 + i],
				game->config.animated_sprite_texture_paths[i]);
		i++;
	}
}

void	load_textures(t_game *game)
{
	int	total_textures;

	total_textures = 4 + 1 + game->config.num_animated_sprite_frames;
	game->num_textures = total_textures;
	game->textures = ft_calloc(total_textures, sizeof(t_img));
	if (!game->textures)
		exit_error("Malloc failed for textures array", game);
	load_static_textures(game);
	load_animated_textures(game);
}
