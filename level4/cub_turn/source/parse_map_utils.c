/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:28:04 by tssaito           #+#    #+#             */
/*   Updated: 2025/08/24 11:40:34 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Adds a new line of the map to the linked list.
void	add_map_line_to_list(t_list **map_list, char *line, t_game *game)
{
	ft_lstadd_back(map_list, ft_lstnew(line));
	if (!ft_lstlast(*map_list) || !ft_lstlast(*map_list)->content)
	{
		ft_lstclear(map_list, free);
		exit_error("Malloc failed during map parsing", game);
	}
}

// Trims a map line and checks if it's empty.
char	*trim_and_check_empty_line(t_map_line_proc *proc, char **trimmed_line)
{
	*trimmed_line = ft_strtrim(proc->current_line, "\n");
	if (!*trimmed_line)
	{
		ft_lstclear(&proc->map_list, free);
		exit_error("Malloc failed during map parsing", proc->game);
	}
	free(proc->current_line);
	proc->current_line = *trimmed_line;
	if (ft_strlen(proc->current_line) == 0)
	{
		free(proc->current_line);
		return (get_next_line(proc->fd));
	}
	return (NULL);
}

// Adds a processed line to the map list and updates map width.
void	add_line_to_map_list(t_list **map_list, char *current_line,
		t_game *game)
{
	add_map_line_to_list(map_list, current_line, game);
	if ((int)ft_strlen(current_line) > game->map.width)
		game->map.width = ft_strlen(current_line);
}

// Processes a single line of the map data.
char	*process_single_map_line(t_map_line_proc *proc)
{
	char	*trimmed_line;
	char	*next_line;

	next_line = trim_and_check_empty_line(proc, &trimmed_line);
	if (next_line)
		return (next_line);
	add_line_to_map_list(&proc->map_list, trimmed_line, proc->game);
	return (get_next_line(proc->fd));
}

// Main loop for processing all map lines from the file.
void	process_map_lines_loop(t_map_line_proc *proc)
{
	while (proc->current_line)
	{
		proc->current_line = process_single_map_line(proc);
	}
}
