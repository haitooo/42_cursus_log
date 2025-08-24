/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_door_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:27:44 by tssaito           #+#    #+#             */
/*   Updated: 2025/08/24 11:40:18 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Initializes the data for a single door.
void	init_single_door_data(t_game *game, int i, int j, int *door_idx)
{
	game->doors[*door_idx].x = j;
	game->doors[*door_idx].y = i;
	game->doors[*door_idx].is_open = 0;
	game->doors[*door_idx].open_progress = 0.0;
	game->doors[*door_idx].is_animating = 0;
	(*door_idx)++;
}
