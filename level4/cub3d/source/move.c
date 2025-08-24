/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:27:33 by tssaito           #+#    #+#             */
/*   Updated: 2025/08/24 11:40:07 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Checks if a door at the given coordinates is open.
int	is_door_open(t_game *game, int x, int y)
{
	int	i;

	i = 0;
	while (i < game->num_doors)
	{
		if (game->doors[i].x == x && game->doors[i].y == y)
		{
			return (game->doors[i].is_open);
		}
		i++;
	}
	return (0);
}

// Toggles the state of a door (open/closed) and starts its animation.
static void	toggle_door_state(t_game *game, int map_x, int map_y)
{
	int	i;

	i = -1;
	while (++i < game->num_doors)
	{
		if (game->doors[i].x == map_x && game->doors[i].y == map_y)
		{
			game->doors[i].is_open = !game->doors[i].is_open;
			game->doors[i].is_animating = 1;
			break ;
		}
	}
}

// Handles player interaction with doors.
void	handle_door_interaction(t_game *game)
{
	double	door_check_x;
	double	door_check_y;
	int		map_x;
	int		map_y;

	door_check_x = game->player.pos_x + game->player.dir_x * 0.6;
	door_check_y = game->player.pos_y + game->player.dir_y * 0.6;
	map_x = (int)door_check_x;
	map_y = (int)door_check_y;
	if (map_x < 0 || map_x >= game->map.width || map_y < 0
		|| map_y >= game->map.height)
		return ;
	if (game->map.grid[map_y][map_x] == 'D')
	{
		toggle_door_state(game, map_x, map_y);
	}
}

// Checks if the player is far enough from open doors
// to close them automatically.
void	check_and_close_doors(t_game *game)
{
	int		i;
	double	distance;

	i = 0;
	while (i < game->num_doors)
	{
		if (game->doors[i].is_open && !game->doors[i].is_animating)
		{
			distance = sqrt(pow(game->player.pos_x - (game->doors[i].x + 0.5),
						2) + pow(game->player.pos_y - (game->doors[i].y + 0.5),
						2));
			if (distance > 1.3)
			{
				game->doors[i].is_open = 0;
				game->doors[i].is_animating = 1;
			}
		}
		i++;
	}
}
