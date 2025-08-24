/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map_data.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:27:58 by tssaito           #+#    #+#             */
/*   Updated: 2025/08/24 11:40:30 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Parses the map data from the file descriptor.
void	parse_map(int fd, char *line, t_game *game)
{
	t_list	*map_list;

	map_list = read_map_lines_to_list(fd, line, game);
	init_map_grid(game);
	populate_map_grid(map_list, game);
}

// Handles a line that is identified as part of the map data.
int	handle_map_line(int fd, char *line, t_game *game)
{
	parse_map(fd, line, game);
	return (1);
}

// Reads all map lines from the file into a linked list.
t_list	*read_map_lines_to_list(int fd, char *line, t_game *game)
{
	t_map_line_proc	proc;

	proc.map_list = NULL;
	proc.current_line = line;
	proc.fd = fd;
	proc.game = game;
	process_map_lines_loop(&proc);
	game->map.height = ft_lstsize(proc.map_list);
	return (proc.map_list);
}

// Initializes the map grid with the appropriate height.
void	init_map_grid(t_game *game)
{
	int	i;

	game->map.grid = malloc(sizeof(char *) * (game->map.height + 1));
	if (!game->map.grid)
		exit_error("Malloc failed for map grid", game);
	i = 0;
	while (i < game->map.height)
	{
		game->map.grid[i] = NULL;
		i++;
	}
	game->map.grid[game->map.height] = NULL;
}

// Populates the map grid from the linked list of map lines.
void	populate_map_grid(t_list *map_list, t_game *game)
{
	int	i;

	i = 0;
	while (map_list)
	{
		process_map_list_node(game, &map_list, &i);
	}
}
