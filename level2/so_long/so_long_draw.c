/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_draw.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 00:25:03 by haito             #+#    #+#             */
/*   Updated: 2025/02/28 07:28:39 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	draw_heart(t_deta *d)
{
	void	*t;
	int		count;

	count = 1;
	while (count < 6)
	{
		t = select_heart_tex(d, count);
		if (!t)
			return ;
		if (mlx_put_image_to_window(d->win->mlx, d->win->wind,
				t, (10 * STATUS_WIDTH) - 95 + (count * IMG_WIDTH) - (count * 8),
				NUM_OF_TILEY * IMG_HEIGHT + (STATUS_HEIGHT / 6)) == -1)
			key_esc(d);
		count++;
	}
}

void	update_location(t_deta *d)
{
	if (d->map->x < d->map->current_map_x
		|| d->map->x >= d->map->current_map_x + NUM_OF_TILEX
		|| d->map->y < d->map->current_map_y
		|| d->map->y >= d->map->current_map_y + NUM_OF_TILEY)
	{
		d->map->current_map_x = (d->map->x / NUM_OF_TILEX) * NUM_OF_TILEX;
		d->map->current_map_y = (d->map->y / NUM_OF_TILEY) * NUM_OF_TILEY;
	}
	d->st->w = d->map->current_map_x;
	d->st->h = d->map->current_map_y;
	d->st->end_x = d->st->w + NUM_OF_TILEX;
	d->st->end_y = d->st->h + NUM_OF_TILEY;
	if (d->st->end_x > d->map->map_width)
		d->st->end_x = d->map->map_width;
	if (d->st->end_y > d->map->map_height)
		d->st->end_y = d->map->map_height;
}

int	draw_map(t_deta *d)
{
	void	*t;

	if (++d->st->count_frame > INT_MAX)
		d->st->count_frame = 0;
	update_location(d);
	while (d->st->h < d->st->end_y)
	{
		while (d->st->w < d->st->end_x)
		{
			t = select_tex(d, d->map->objs[d->st->h][d->st->w]);
			if (mlx_put_image_to_window(d->win->mlx, d->win->wind,
					t, (d->st->w - d->map->current_map_x) * IMG_WIDTH,
					(d->st->h - d->map->current_map_y) * IMG_HEIGHT) == -1)
				key_esc(d);
			d->st->w++;
		}
		d->st->w = d->map->current_map_x;
		d->st->h++;
	}
	draw_status(d);
	return (0);
}
