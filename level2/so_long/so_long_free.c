/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_free.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 05:31:26 by haito             #+#    #+#             */
/*   Updated: 2025/02/22 09:32:46 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	free_objs(t_map *map, int obj_size)
{
	int	n;

	n = 0;
	if (!map->objs)
		return ;
	while (n < map->map_height && n < obj_size)
	{
		if (map->objs[n])
		{
			free(map->objs[n]);
			map->objs[n] = NULL;
		}
		n++;
	}
	free(map->objs);
	map->objs = NULL;
}

void	free_tmp_map(t_map *map, char **tmp_map)
{
	int	n;

	n = -1;
	while (++n < map->map_height)
		free(tmp_map[n]);
	free(tmp_map);
	tmp_map = NULL;
	exit(1);
}
