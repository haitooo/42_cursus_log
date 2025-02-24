#include "so_long.h"

void	*select_tex(t_texture *tex, t_map *map, char c)
{
	if (c == '0')
		return (tex->back_img);
	if (c == '1')
		return (tex->wall_img);
	if (c == 'P')
	{
		if (map->direction == UP)
			return (tex->player_up_img);
		if (map->direction == DOWN)
			return (tex->player_down_img);
		if (map->direction == LEFT)
			return (tex->player_left_img);
		if (map->direction == RIGHT)
			return (tex->player_right_img);
	}
	if (c == 'C')
		return (tex->item_img);
	if (c == 'E')
		return (tex->exit_img);
	return (NULL);
}

void	*select_status2_tex(t_texture *tex, int count)
{
	if (count == 0)
		return (tex->status4_img);
	if (count == 1)
		return (tex->status5_img);
	if (count == 2)
		return (tex->status6_img);
	else
		return (NULL);
}

void	*select_status_tex(t_texture *tex, int count)
{
	if (count == 0)
		return (tex->status1_img);
	if (count == 1)
		return (tex->status2_img);
	if (count == 2)
		return (tex->status3_img);
	else
		return (tex->status_img);
}

void	*select_heart_tex(t_deta *d, int count)
{
	if (d->st->hp - count * 2 >= 0)
		return (d->tex->heart_img);
	if (d->st->hp - count * 2 == -1)
		return (d->tex->heart_half_img);
	if (d->st->hp - count * 2 <= -2)
		return (d->tex->heart_zero_img);
	return (NULL);
}

void	*select_num_tex(t_deta *d, int num)
{
	if (num == 0)
		return (d->tex->n->zero_img);
	if (num == 1)
		return (d->tex->n->one_img);
	if (num == 2)
		return (d->tex->n->two_img);
	if (num == 3)
		return (d->tex->n->three_img);
	if (num == 4)
		return (d->tex->n->four_img);
	if (num == 5)
		return (d->tex->n->five_img);
	if (num == 6)
		return (d->tex->n->six_img);
	if (num == 7)
		return (d->tex->n->seven_img);
	if (num == 8)
		return (d->tex->n->eight_img);
	if (num == 9)
		return (d->tex->n->nine_img);
	return (NULL);
}
