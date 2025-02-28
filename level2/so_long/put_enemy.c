/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_enemy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 18:52:50 by haito             #+#    #+#             */
/*   Updated: 2025/02/28 08:53:17 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

#define MAX_TRY 100

void	put_patrol(t_map *map)
{
	int	num_x;
	int	num_y;
	int	count;
	int	try;

	count = 0;
	num_x = ((__TIME__[7] - '0') + (__TIME__[6] - '0') * 10) % map->map_width;
	num_y = ((__TIME__[4] - '0') + (__TIME__[3] - '0') * 10) % map->map_height;
	try = 0;
	while (map->objs[num_y][num_x] != '0' && try++ <= MAX_TRY)
	{
		num_x++;
		num_x %= map->map_width;
		num_y++;
		num_y %= map->map_height;
	}
	if (try >= MAX_TRY)
		return ;
	if (map->objs[num_y][num_x] == '0')
		map->objs[num_y][num_x] = 'M';
	map->pat_direction = DOWN;
	map->pat_x = num_x;
	map->pat_y = num_y;
	map->num_m++;
}

void	put_trap(t_map *map)
{
	int	num_x;
	int	num_y;
	int	count;
	int	try;

	count = 0;
	num_x = ((__TIME__[6] - '0') + (__TIME__[7] - '0') * 10) % map->map_width;
	num_y = ((__TIME__[3] - '0') + (__TIME__[4] - '0') * 10) % map->map_height;
	while (count < 5)
	{
		try = 0;
		while (map->objs[num_y][num_x] != '0' && try++ <= MAX_TRY)
		{
			num_x++;
			num_x %= map->map_width;
			num_y = (num_y + (__TIME__[7] - '0')) % map->map_height;
		}
		if (try >= MAX_TRY)
			break ;
		if (map->objs[num_y][num_x] == '0')
		{
			map->objs[num_y][num_x] = 'T';
			count++;
		}
	}
}

void	put_enemy(t_map *map)
{
	map->num_m = 0;
	if (MODE == HARD)
		put_patrol(map);
	if (MODE != PEACE)
		put_trap(map);
}
