/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elements.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:27:47 by tssaito           #+#    #+#             */
/*   Updated: 2025/08/24 11:40:23 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Validates that RGB color values are within the valid range of 0-255.
void	validate_color_values(int r, int g, int b, t_game *game)
{
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		exit_error("Color values out of range [0-255]", game);
}

// Parses a texture path from a line in the configuration file.
void	parse_texture_path(char *line, char **path, t_game *game)
{
	char	*tmp;
	char	*trimmed_path;

	tmp = ft_strchr(line, ' ');
	while (tmp && *tmp == ' ')
		tmp++;
	if (!tmp || ft_strlen(tmp) == 0)
		exit_error("Missing texture path", game);
	trimmed_path = ft_strtrim(tmp, " \n");
	if (!trimmed_path)
		exit_error("Malloc failed", game);
	*path = trimmed_path;
}

// Parses a color from a line in the configuration file.
void	parse_color(char *line, int *color, t_game *game)
{
	char			**rgb_str;
	t_rgb_values	rgb;

	get_rgb_strings(line, game, &rgb_str);
	convert_and_validate_rgb(rgb_str, &rgb, game);
	ft_free_split(rgb_str);
	*color = (rgb.r << 16) | (rgb.g << 8) | rgb.b;
}

// Handles a line from the config file that defines a texture.
int	handle_texture_line(char *line, t_game *game)
{
	if (ft_strncmp(line, "NO ", 3) == 0 && !game->config.north_texture_path)
		parse_texture_path(line, &game->config.north_texture_path, game);
	else if (ft_strncmp(line, "SO ", 3) == 0
		&& !game->config.south_texture_path)
		parse_texture_path(line, &game->config.south_texture_path, game);
	else if (ft_strncmp(line, "WE ", 3) == 0 && !game->config.west_texture_path)
		parse_texture_path(line, &game->config.west_texture_path, game);
	else if (ft_strncmp(line, "EA ", 3) == 0 && !game->config.east_texture_path)
		parse_texture_path(line, &game->config.east_texture_path, game);
	else if (ft_strncmp(line, "DO ", 3) == 0 && !game->config.door_texture_path)
		parse_texture_path(line, &game->config.door_texture_path, game);
	else if (line[0] == 'X' && ft_isdigit(line[1]))
		parse_animated_sprite_texture_path(line, game);
	else
		return (0);
	return (1);
}

// Handles a line from the config file that defines a color.
int	handle_color_line(char *line, t_game *game)
{
	if (ft_strncmp(line, "F ", 2) == 0 && game->config.floor_color == -1)
		parse_color(line, &game->config.floor_color, game);
	else if (ft_strncmp(line, "C ", 2) == 0 && game->config.ceiling_color == -1)
		parse_color(line, &game->config.ceiling_color, game);
	else
		return (0);
	return (1);
}
