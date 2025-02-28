/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 00:16:53 by haito             #+#    #+#             */
/*   Updated: 2025/02/28 09:08:44 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	found_player3(t_map *map)
{
	if (map->pat_x > map->x && map->pat_y > map->y)
	{
		if (map->objs[map->y - 1][map->x] == 'D'
			|| map->objs[map->y + 1][map->x] == 'D')
			return ;
		if (map->objs[map->pat_y - 1][map->pat_x] == '0')
			p_up(map);
		else if (map->objs[map->pat_y][map->pat_x - 1] == '0')
			p_left(map);
	}
	else if (map->pat_x > map->x && map->pat_y < map->y)
	{
		if (map->objs[map->y - 1][map->x] == 'D'
			|| map->objs[map->y + 1][map->x] == 'D')
			return ;
		if (map->objs[map->pat_y + 1][map->pat_x] == '0')
			p_down(map);
		else if (map->objs[map->pat_y][map->pat_x - 1] == '0')
			p_left(map);
	}
}

void	found_player2(t_map *map)
{
	if (map->pat_x < map->x && map->pat_y < map->y)
	{
		if (map->objs[map->y][map->x - 1] == 'D'
			|| map->objs[map->y][map->x + 1] == 'D')
			return ;
		if (map->objs[map->pat_y][map->pat_x + 1] == '0')
			p_right(map);
		else if (map->objs[map->pat_y + 1][map->pat_x] == '0')
			p_down(map);
	}
	else if (map->pat_x < map->x && map->pat_y > map->y)
	{
		if (map->objs[map->y][map->x - 1] == 'D'
			|| map->objs[map->y][map->x + 1] == 'D')
			return ;
		if (map->objs[map->pat_y][map->pat_x + 1] == '0')
			p_right(map);
		else if (map->objs[map->pat_y - 1][map->pat_x] == '0')
			p_up(map);
	}
	else
		found_player3(map);
}

void	found_player(t_map *map)
{
	if (map->pat_x < map->x && map->pat_y == map->y)
	{
		if (map->objs[map->pat_y][map->pat_x + 1] == '0')
			p_right(map);
	}
	else if (map->pat_x > map->x && map->pat_y == map->y)
	{
		if (map->objs[map->pat_y][map->pat_x - 1] == '0')
			p_left(map);
	}
	else if (map->pat_x == map->x && map->pat_y > map->y)
	{
		if (map->objs[map->pat_y - 1][map->pat_x] == '0')
			p_up(map);
	}
	else if (map->pat_x == map->x && map->pat_y < map->y)
	{
		if (map->objs[map->pat_y + 1][map->pat_x] == '0')
			p_down(map);
	}
	else
		found_player2(map);
}

int	search_player_x(t_map *map, int search_y)
{
	int	x_min;
	int	x_max;
	int	search_x;

	if (map->pat_x - 3 < 0)
		x_min = 0;
	else
		x_min = map->pat_x - 3;
	if (map->pat_x + 3 >= map->map_width)
		x_max = map->map_width - 1;
	else
		x_max = map->pat_x + 3;
	search_x = x_min;
	while (++search_x <= x_max)
	{
		if (map->objs[search_y][search_x] == 'P')
		{
			if (map->objs[map->pat_y][map->pat_x] != '1')
				map->objs[map->pat_y][map->pat_x] = 'D';
			found_player(map);
			return (1);
		}
	}
	return (0);
}

void	search_player(t_map *map)
{
	int	search_y;
	int	y_min;
	int	y_max;

	if (map->pat_y - 3 < 0)
		y_min = 0;
	else
		y_min = map->pat_y - 3;
	if (map->pat_y + 3 >= map->map_height)
		y_max = map->map_height - 1;
	else
		y_max = map->pat_y + 3;
	search_y = y_min;
	while (++search_y <= y_max)
	{
		if (search_player_x(map, search_y) == 1)
			return ;
	}
}
