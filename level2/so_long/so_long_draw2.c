/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_draw2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 00:25:55 by haito             #+#    #+#             */
/*   Updated: 2025/02/28 06:59:26 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	draw_total_item(t_deta *d)
{
	void	*t;
	int		count;
	int		locate;
	int		num;

	count = 0;
	locate = 2;
	while (count < 3)
	{
		if (d->map->num_c >= 999)
			num = 9;
		else if (locate > 0)
			num = (d->map->num_c / my_power(10, locate)) % 10;
		else
			num = d->map->num_c % 10;
		t = select_num_tex(d, num);
		if (!t)
			return ;
		if (mlx_put_image_to_window(d->win->mlx, d->win->wind,
				t, (8 * STATUS_WIDTH) + 53 + (count * IMG_WIDTH) - (count * 23),
				NUM_OF_TILEY * IMG_HEIGHT + (STATUS_HEIGHT / 6)) == -1)
			key_esc(d);
		count++;
		locate--;
	}
}

void	draw_got_item(t_deta *d)
{
	void	*t;
	int		count;
	int		locate;
	int		num;

	count = 0;
	locate = 2;
	while (count < 3)
	{
		if (d->st->count_got_item >= 999)
			num = 9;
		else if (locate > 0)
			num = (d->st->count_got_item / my_power(10, locate)) % 10;
		else
			num = d->st->count_got_item % 10;
		t = select_num_tex(d, num);
		if (!t)
			return ;
		if (mlx_put_image_to_window(d->win->mlx, d->win->wind,
				t, (7 * STATUS_WIDTH) + (count * IMG_WIDTH) - (75 + count * 23),
				NUM_OF_TILEY * IMG_HEIGHT + (STATUS_HEIGHT / 6)) == -1)
			key_esc(d);
		count++;
		locate--;
	}
}

void	draw_item(t_deta *d)
{
	void	*t;
	int		count;

	count = 0;
	while (count < 3)
	{
		t = select_status2_tex(d->tex, count);
		mlx_put_image_to_window(d->win->mlx, d->win->wind,
			t, ((count + 4) * STATUS_WIDTH + 48),
			NUM_OF_TILEY * IMG_HEIGHT);
		count++;
	}
	if (mlx_put_image_to_window(d->win->mlx, d->win->wind,
			(void *)d->tex->n->par_img,
			8 * STATUS_WIDTH - 16, NUM_OF_TILEY * IMG_HEIGHT) == -1)
		key_esc(d);
	draw_got_item(d);
	draw_total_item(d);
}

void	draw_score(t_deta *d)
{
	void	*t;
	int		count;
	int		locate;
	int		num;

	count = 0;
	locate = 3;
	while (count < 4)
	{
		if (d->st->count_step >= 9999)
			num = 9;
		else if (locate > 0)
			num = (d->st->count_step / my_power(10, locate)) % 10;
		else
			num = d->st->count_step % 10;
		t = select_num_tex(d, num);
		if (!t)
			return ;
		if (mlx_put_image_to_window(d->win->mlx, d->win->wind,
				t, (3 * STATUS_WIDTH) + (count * IMG_WIDTH) - (40 + count * 23),
				NUM_OF_TILEY * IMG_HEIGHT + (STATUS_HEIGHT / 6)) == -1)
			key_esc(d);
		count++;
		locate--;
	}
}

void	draw_status(t_deta *d)
{
	void	*t;
	int		count;

	count = 0;
	while (count < d->win->win_w / STATUS_WIDTH + 1)
	{
		t = select_status_tex(d->tex, count);
		if (mlx_put_image_to_window(d->win->mlx, d->win->wind,
				t, (count * STATUS_WIDTH),
				NUM_OF_TILEY * IMG_HEIGHT) == -1)
			key_esc(d);
		count++;
	}
	draw_item(d);
	draw_score(d);
	draw_heart(d);
}
