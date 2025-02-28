/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_move.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 00:14:41 by haito             #+#    #+#             */
/*   Updated: 2025/02/28 09:08:56 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	p_right(t_map *map)
{
	map->objs[map->pat_y][map->pat_x + 1] = 'D';
	map->objs[map->pat_y][map->pat_x] = '0';
	map->pat_direction = RIGHT;
	map->pat_x++;
}

void	p_left(t_map *map)
{
	map->objs[map->pat_y][map->pat_x - 1] = 'D';
	map->objs[map->pat_y][map->pat_x] = '0';
	map->pat_direction = LEFT;
	map->pat_x--;
}

void	p_up(t_map *map)
{
	map->objs[map->pat_y - 1][map->pat_x] = 'D';
	map->objs[map->pat_y][map->pat_x] = '0';
	map->pat_direction = UP;
	map->pat_y--;
}

void	p_down(t_map *map)
{
	map->objs[map->pat_y + 1][map->pat_x] = 'D';
	map->objs[map->pat_y][map->pat_x] = '0';
	map->pat_direction = DOWN;
	map->pat_y++;
}

void	check_patrol(t_deta *d)
{
	if (MODE != HARD)
		return ;
	search_player(d->map);
}
