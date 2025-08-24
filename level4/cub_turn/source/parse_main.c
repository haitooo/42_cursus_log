/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:27:53 by tssaito           #+#    #+#             */
/*   Updated: 2025/08/24 11:40:28 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Processes a single line from the .cub file,
//	dispatching to appropriate handlers.
int	process_cub_line(char *line, t_game *game, int *elements_count, int fd)
{
	int	is_map_line;

	is_map_line = 0;
	if (ft_strlen(line) == 0)
		return (1);
	if (handle_texture_line(line, game))
		(*elements_count)++;
	else if (handle_color_line(line, game))
		(*elements_count)++;
	else if (*elements_count >= 6)
		is_map_line = handle_map_line(fd, line, game);
	else
		exit_error("Invalid line in cub file or missing configuration elements",
			game);
	return (!is_map_line);
}

// Main parsing function that reads and processes the entire .cub file.
void	parse_cub_file(char *file_path, t_game *game)
{
	int		fd;
	char	*line;
	int		elements_count;

	fd = open(file_path, O_RDONLY);
	if (fd < 0)
		exit_error("Could not open .cub file", game);
	elements_count = 0;
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (process_cub_line(line, game, &elements_count, fd))
			free(line);
		else
			break ;
	}
	close(fd);
	if (elements_count < 6)
		exit_error("Missing elements in .cub file", game);
	validate_map(game);
}
