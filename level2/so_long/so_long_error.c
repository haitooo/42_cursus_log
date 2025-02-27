/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_error.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 08:39:44 by haito             #+#    #+#             */
/*   Updated: 2025/02/28 06:28:10 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	error_map(t_map *map, int n, int errornum)
{
	if (errornum == ERRNO_WALL)
		ft_dprintf("Map error: A wall is damaged somewhere\n");
	if (errornum == ERRNO_DUP)
		ft_dprintf("Map error: Too many P, E or has invalid character\n");
	if (errornum == ERRNO_SHAPE)
		ft_dprintf("Map error: Maps are rectangular or square\n");
	if (errornum == ERRNO_FEW_C)
		ft_dprintf("Map error: No item!! No game!!\n");
	free_objs(map, n);
	exit(1);
}

void	error_malloc(t_map *map, int n, char **tmp_map, int i)
{
	int	count;

	count = -1;
	ft_dprintf("Error: malloc failed\n");
	if (tmp_map)
	{
		while (++count < i)
		{
			free(tmp_map[count]);
			tmp_map[count] = NULL;
		}
		free(tmp_map);
	}
	free_objs(map, n);
	exit(1);
}

void	error_hook(t_window *win, t_map *map, t_deta *deta, int errnum)
{
	ft_dprintf("Error: %s\n", strerror(errno));
	if (win->wind)
		mlx_destroy_window(win->mlx, win->wind);
	if (win->mlx)
	{
		mlx_destroy_display(win->mlx);
		free(win->mlx);
	}
	free_objs(map, map->map_height);
	if (errnum == ERRNO_HOOK)
		free(deta);
	exit (1);
}

void	error_import(t_deta *d)
{
	ft_dprintf("Error: %s\n", strerror(errno));
	destroy_imgs(d);
	if (d->win->wind)
		mlx_destroy_window(d->win->mlx, d->win->wind);
	if (d->win->mlx)
	{
		mlx_destroy_display(d->win->mlx);
		free(d->win->mlx);
	}
	free_objs(d->map, d->map->map_height);
	if (d)
		free(d);
	exit (1);
}
