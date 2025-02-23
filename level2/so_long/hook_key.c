/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook_key.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 21:41:59 by haito             #+#    #+#             */
/*   Updated: 2025/02/22 15:49:48 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int	key_esc(t_deta *deta)
{
	destroy_imgs(deta);
	if (deta->win->wind)
		mlx_destroy_window(deta->win->mlx, deta->win->wind);
	if (deta->win->mlx)
	{
		mlx_destroy_display(deta->win->mlx);
		free(deta->win->mlx);
	}
	free_objs(deta->map, deta->map->map_height);
	if (deta)
		free(deta);
	exit(0);
	return (1);
}

void	call_keypress_func(int keynum, t_deta *deta)
{
	if (keynum == ESC_KEY || keynum == X_CLICK)
		key_esc(deta);
	if (keynum == W_KEY || keynum == UP_KEY)
		key_up(deta);
	if (keynum == S_KEY || keynum == DOWN_KEY)
		key_down(deta);
	if (keynum == A_KEY || keynum == LEFT_KEY)
		key_left(deta);
	if (keynum == D_KEY || keynum == RIGHT_KEY)
		key_right(deta);
}
