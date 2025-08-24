/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_animated_elements.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:27:39 by tssaito           #+#    #+#             */
/*   Updated: 2025/08/24 11:40:13 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Extracts the frame number and path for an animated sprite from a line of text
void	get_animated_sprite_frame_num_and_path(char *line, t_game *game,
		char ***parts, int *frame_num)
{
	*parts = ft_split(line, ' ');
	if (!*parts || !(*parts)[0] || !(*parts)[1] || (*parts)[2])
	{
		ft_free_split(*parts);
		exit_error("Invalid animated sprite texture format", game);
	}
	*frame_num = ft_atoi((*parts)[0] + 1);
	if (*frame_num <= 0)
	{
		ft_free_split(*parts);
		exit_error("Animated sprite frame number must be a positive integer",
			game);
	}
}

// Reallocates memory for animated sprite paths if necessary.
void	reallocate_animated_sprite_paths(t_game *game, int frame_num)
{
	char	**new_paths;
	int		new_capacity;
	int		i;

	if (frame_num <= game->config.animated_sprite_paths_capacity)
		return ;
	new_capacity = frame_num;
	new_paths = ft_calloc(new_capacity, sizeof(char *));
	if (!new_paths)
		exit_error("Malloc failed for animated sprite paths", game);
	i = 0;
	if (game->config.animated_sprite_texture_paths)
	{
		while (i < game->config.animated_sprite_paths_capacity)
		{
			new_paths[i] = game->config.animated_sprite_texture_paths[i];
			i++;
		}
		free(game->config.animated_sprite_texture_paths);
	}
	game->config.animated_sprite_texture_paths = new_paths;
	game->config.animated_sprite_paths_capacity = new_capacity;
}

// Parses the texture path for an animated sprite from a line in the config file
void	parse_animated_sprite_texture_path(char *line, t_game *game)
{
	char	**parts;
	char	*trimmed_path;
	int		frame_num;

	get_animated_sprite_frame_num_and_path(line, game, &parts, &frame_num);
	reallocate_animated_sprite_paths(game, frame_num);
	trimmed_path = ft_strtrim(parts[1], " \n");
	if (!trimmed_path)
	{
		ft_free_split(parts);
		exit_error("Malloc failed", game);
	}
	if (game->config.animated_sprite_texture_paths[frame_num - 1])
		free(game->config.animated_sprite_texture_paths[frame_num - 1]);
	game->config.animated_sprite_texture_paths[frame_num - 1] = trimmed_path;
	if (frame_num > game->config.num_animated_sprite_frames)
		game->config.num_animated_sprite_frames = frame_num;
	ft_free_split(parts);
}
