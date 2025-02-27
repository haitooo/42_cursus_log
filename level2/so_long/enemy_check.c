/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 00:16:53 by haito             #+#    #+#             */
/*   Updated: 2025/02/28 04:52:56 by haito            ###   ########.fr       */
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

void	search_player(t_map *map)
{
	int	search_x;
	int	search_y;

	search_y = map->pat_y - 3;
	while (search_y < 0)
		search_y = 0;
	while (search_y < map->pat_y + 3)
	{
		search_x = map->pat_x - 3;
		while (search_x < 0)
			search_x = 0;
		while (search_x < map->pat_x + 3)
		{
			if (map->objs[search_y][search_x] == 'P')
			{
				map->objs[map->pat_y][map->pat_x] = 'D';
				found_player(map);
				return ;
			}
			if (++search_x > map->map_width)
				break ;
		}
		if (++search_y > map->map_height)
			break ;
	}
}

void	check_patrol(t_deta *d)
{
	if (MODE != HARD)
		return ;
	search_player(d->map);
}
