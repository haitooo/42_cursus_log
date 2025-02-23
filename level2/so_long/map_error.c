/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_error.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 07:05:30 by haito             #+#    #+#             */
/*   Updated: 2025/02/22 12:59:37 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	map_explore(t_map *map, char **tmp_map, int x, int y)
{
	if (x > map->map_width - 2 || y > map->map_height - 2
		|| x < 1 || y < 1)
		return ;
	tmp_map[y][x] = 'd';
	if (tmp_map[y - 1][x] != '1' && tmp_map[y - 1][x] != 'd')
		map_explore(map, tmp_map, x, y - 1);
	if (tmp_map[y + 1][x] != '1' && tmp_map[y + 1][x] != 'd')
		map_explore(map, tmp_map, x, y + 1);
	if (tmp_map[y][x - 1] != '1' && tmp_map[y][x - 1] != 'd')
		map_explore(map, tmp_map, x - 1, y);
	if (tmp_map[y][x + 1] != '1' && tmp_map[y][x + 1] != 'd')
		map_explore(map, tmp_map, x + 1, y);
	return ;
}

void	has_item(t_map *map, char **tmp_map)
{
	int	n;
	int	i;

	n = -1;
	while (++n < map->map_height)
	{
		i = -1;
		while (++i < map->map_width)
		{
			if (tmp_map[n][i] == 'E' || tmp_map[n][i] == 'P'
				|| tmp_map[n][i] == 'C')
			{
				ft_dprintf("The map has unreachable object\n");
				free_objs(map, map->map_height);
				free_tmp_map(map, tmp_map);
			}
		}
	}
}

void	check_isolated_obj(t_map *map)
{
	int		n;
	int		i;
	char	**tmp_map;

	tmp_map = malloc(sizeof(char *) * map->map_height);
	if (!tmp_map)
		error_malloc(map, map->map_height, NULL, 0);
	n = -1;
	while (++n < map->map_height)
	{
		tmp_map[n] = malloc(sizeof(char) * map->map_width);
		if (!tmp_map[n])
			error_malloc(map, map->map_height, tmp_map, n);
		i = -1;
		while (++i < map->map_width)
			tmp_map[n][i] = map->objs[n][i];
	}
	map_explore(map, tmp_map, map->x, map->y);
	has_item(map, tmp_map);
	n = -1;
	while (++n < map->map_height)
		free(tmp_map[n]);
	free(tmp_map);
	tmp_map = NULL;
}

int	check_wall(t_map *map)
{
	int	n;

	n = -1;
	while (++n < map->map_width)
	{
		if (map->objs[0][n] != '1' || map->objs[map->map_height - 1][n] != '1')
			return (-1);
	}
	return (0);
}

void	check_map_error(t_map *map, char *mapname)
{
	int	n;

	n = 0;
	map->objs = read_map(map, mapname);
	if (!map->objs)
	{
		ft_dprintf("Error: %s\n", strerror(errno));
		exit(1);
	}
	if (check_wall(map) == -1)
		error_map(map, map->map_height, ERRNO_WALL);
	while (n < map->map_height)
	{
		if (ft_int_strlen(map->objs[n]) != map->map_width)
			error_map(map, map->map_height, ERRNO_SHAPE);
		if (map->objs[n][0] != '1' || map->objs[n][map->map_width - 1] != '1')
			error_map(map, map->map_height, ERRNO_WALL);
		if (count_object(map, n) == -1)
			error_map(map, map->map_height, ERRNO_DUP);
		n++;
	}
	if (map->num_c <= 0)
		error_map(map, map->map_height, ERRNO_FEW_C);
	check_isolated_obj(map);
}
