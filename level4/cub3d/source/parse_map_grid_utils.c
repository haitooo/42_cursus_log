/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map_grid_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:28:01 by tssaito           #+#    #+#             */
/*   Updated: 2025/08/24 13:03:42 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Allocates memory for a map row and copies the content from the linked list.
void	allocate_and_copy_row(t_game *game, t_list *map_list, int i)
{
	game->map.grid[i] = malloc(sizeof(char) * (game->map.width + 1));
	if (!game->map.grid[i])
	{
		ft_lstclear(&map_list, free);
		exit_error("Malloc failed for map row", game);
	}
	ft_memset(game->map.grid[i], ' ', game->map.width);
	ft_memcpy(game->map.grid[i], map_list->content,
		ft_strlen(map_list->content));
	game->map.grid[i][game->map.width] = '\0';
}

// Processes a single node from the map list, creating a row in the map grid.
void	process_map_list_node(t_game *game, t_list **map_list, int *i)
{
	t_list	*temp;

	allocate_and_copy_row(game, *map_list, *i);
	temp = *map_list;
	*map_list = (*map_list)->next;
	ft_lstdelone(temp, free);
	(*i)++;
}
