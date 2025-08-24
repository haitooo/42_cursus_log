/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_color_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:27:42 by tssaito           #+#    #+#             */
/*   Updated: 2025/08/24 11:55:48 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Checks if the string is empty or has a leading zero.
static void	check_color_str_format(char *trimmed_str, t_game *game)
{
	if (ft_strlen(trimmed_str) == 0)
	{
		free(trimmed_str);
		exit_error("Color value is empty", game);
	}
	if (ft_strlen(trimmed_str) > 1 && trimmed_str[0] == '0')
	{
		free(trimmed_str);
		exit_error("Color value has a leading zero", game);
	}
}

// Checks if all characters in the string are digits.
static void	check_color_str_digits(char *trimmed_str, t_game *game)
{
	int	i;

	i = 0;
	while (trimmed_str[i])
	{
		if (!ft_isdigit(trimmed_str[i]))
		{
			free(trimmed_str);
			exit_error("Color value contains non-digit characters", game);
		}
		i++;
	}
}

// Top-level validation function for a single color string.
static void	validate_rgb_string(char *str, t_game *game)
{
	char	*trimmed_str;

	trimmed_str = ft_strtrim(str, " \t\n\v\f\r");
	if (!trimmed_str)
		exit_error("Malloc failed during color validation", game);
	check_color_str_format(trimmed_str, game);
	check_color_str_digits(trimmed_str, game);
	free(trimmed_str);
}

// Extracts RGB color strings from a line of text.
void	get_rgb_strings(char *line, t_game *game, char ***rgb_str)
{
	char	*tmp;

	tmp = ft_strchr(line, ' ');
	while (tmp && *tmp == ' ')
		tmp++;
	if (!tmp || ft_strlen(tmp) == 0)
		exit_error("Missing color values", game);
	*rgb_str = ft_split(tmp, ',');
	if (!*rgb_str || !(*rgb_str)[0] || !(*rgb_str)[1] || !(*rgb_str)[2]
		|| (*rgb_str)[3])
	{
		ft_free_split(*rgb_str);
		exit_error("Invalid color format", game);
	}
}

// Converts RGB string values to integers and validates them.
void	convert_and_validate_rgb(char **rgb_str, t_rgb_values *rgb,
		t_game *game)
{
	validate_rgb_string(rgb_str[0], game);
	validate_rgb_string(rgb_str[1], game);
	validate_rgb_string(rgb_str[2], game);
	rgb->r = ft_atoi(rgb_str[0]);
	rgb->g = ft_atoi(rgb_str[1]);
	rgb->b = ft_atoi(rgb_str[2]);
	validate_color_values(rgb->r, rgb->g, rgb->b, game);
}
