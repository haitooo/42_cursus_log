/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_free_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:26:23 by tssaito           #+#    #+#             */
/*   Updated: 2025/08/24 12:43:24 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Frees memory allocated for texture paths in the game configuration.
void	free_config_paths(t_game *game)
{
	if (game->config.north_texture_path)
		free(game->config.north_texture_path);
	if (game->config.south_texture_path)
		free(game->config.south_texture_path);
	if (game->config.west_texture_path)
		free(game->config.west_texture_path);
	if (game->config.east_texture_path)
		free(game->config.east_texture_path);
	if (game->config.door_texture_path)
		free(game->config.door_texture_path);
}

// Frees memory allocated for animated sprite texture paths.
void	free_animated_sprites(t_game *game)
{
	int	i;

	if (game->config.animated_sprite_texture_paths)
	{
		i = 0;
		while (i < game->config.num_animated_sprite_frames)
		{
			if (game->config.animated_sprite_texture_paths[i])
				free(game->config.animated_sprite_texture_paths[i]);
			i++;
		}
		free(game->config.animated_sprite_texture_paths);
	}
}

// Frees all memory related to the game map and configuration.
void	free_game_map_config(t_game *game)
{
	int	i;

	if (game->map.grid)
	{
		i = 0;
		while (i < game->map.height)
		{
			free(game->map.grid[i]);
			i++;
		}
		free(game->map.grid);
	}
	free_config_paths(game);
	free_animated_sprites(game);
	if (game->doors)
		free(game->doors);
	if (game->sprites)
		free(game->sprites);
	if (game->z_buffer)
		free(game->z_buffer);
}

// Frees memory allocated for game images and textures.
void	free_game_images(t_game *game)
{
	int	i;

	if (game->img.img_ptr)
		mlx_destroy_image(game->mlx, game->img.img_ptr);
	if (game->textures)
	{
		i = 0;
		while (i < game->num_textures)
		{
			if (game->textures[i].img_ptr)
				mlx_destroy_image(game->mlx, game->textures[i].img_ptr);
			i++;
		}
		free(game->textures);
	}
}

// Frees MLX-related resources like the window and display.
void	free_game_mlx(t_game *game)
{
	if (game->win)
		mlx_destroy_window(game->mlx, game->win);
	if (game->mlx)
		mlx_destroy_display(game->mlx);
}
