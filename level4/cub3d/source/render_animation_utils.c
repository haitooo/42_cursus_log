/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_animation_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:28:29 by tssaito           #+#    #+#             */
/*   Updated: 2025/08/24 11:40:42 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Updates the animation progress for a single door.
void	update_single_door_animation(t_game *game, int i)
{
	if (game->doors[i].is_open)
	{
		game->doors[i].open_progress += 0.1;
		if (game->doors[i].open_progress >= 1.0)
		{
			game->doors[i].open_progress = 1.0;
			game->doors[i].is_animating = 0;
		}
	}
	else
	{
		game->doors[i].open_progress -= 0.1;
		if (game->doors[i].open_progress <= 0.0)
		{
			game->doors[i].open_progress = 0.0;
			game->doors[i].is_animating = 0;
		}
	}
}
