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
		mlx_put_image_to_window(d->win->mlx, d->win->wind,
			t, (8 * STATUS_WIDTH) + 80 + (count * IMG_WIDTH) - (count * 23),
			NUM_OF_TILEY * IMG_HEIGHT + (STATUS_HEIGHT / 6));
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
		mlx_put_image_to_window(d->win->mlx, d->win->wind,
			t, (7 * STATUS_WIDTH) + (count * IMG_WIDTH) - (53 + count * 23),
			NUM_OF_TILEY * IMG_HEIGHT + (STATUS_HEIGHT / 6));
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
			t, ((count + 4) * STATUS_WIDTH + 70),
			NUM_OF_TILEY * IMG_HEIGHT);
		count++;
	}
	mlx_put_image_to_window(d->win->mlx, d->win->wind,
		(void *)d->tex->par_img,
		8 * STATUS_WIDTH + 10, NUM_OF_TILEY * IMG_HEIGHT);
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
		mlx_put_image_to_window(d->win->mlx, d->win->wind,
			t, (3 * STATUS_WIDTH) + (count * IMG_WIDTH) - (40 + count * 23),
			NUM_OF_TILEY * IMG_HEIGHT + (STATUS_HEIGHT / 6));
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
		mlx_put_image_to_window(d->win->mlx, d->win->wind,
			t, (count * STATUS_WIDTH),
			NUM_OF_TILEY * IMG_HEIGHT);
		count++;
	}
	draw_item(d);
	draw_score(d);
	//char	str[] = "1";
	//mlx_string_put(d->win->mlx, d->win->wind, IMG_WIDTH * 5, NUM_OF_TILEY * IMG_HEIGHT + 45, 0xFFFFFF, str);
	//mlx_put_image_to_window(d->win->mlx, d->win->wind, (void *)d->tex->heart_img, IMG_WIDTH * 6, NUM_OF_TILEY * IMG_HEIGHT + (STATUS_HEIGHT / 6));
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
	d->st->end_y = d->st->h + NUM_OF_TILEX;
	if (d->st->end_x > d->map->map_width)
		d->st->end_x = d->map->map_width;
	if (d->st->end_y > d->map->map_height)
		d->st->end_y = d->map->map_height;
}

int	draw_map(t_deta *d)
{
	void	*t;

	update_location(d);
	mlx_clear_window(d->win->mlx, d->win->wind);
	while (d->st->h < d->st->end_y)
	{
		while (d->st->w < d->st->end_x)
		{
			t = select_tex(d->tex, d->map, d->map->objs[d->st->h][d->st->w]);
			mlx_put_image_to_window(d->win->mlx, d->win->wind,
				t, (d->st->w - d->map->current_map_x) * IMG_WIDTH,
				(d->st->h - d->map->current_map_y) * IMG_HEIGHT);
			d->st->w++;
		}
		d->st->w = d->map->current_map_x;
		d->st->h++;
	}
	draw_status(d);
	return (0);
}
