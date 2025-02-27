/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_select_tex2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 05:12:45 by haito             #+#    #+#             */
/*   Updated: 2025/02/28 08:09:51 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	*case_player(t_deta *d)
{
	if (d->map->direction == UP)
		return (d->tex->player_up_img);
	if (d->map->direction == DOWN)
	{
		if (d->st->count_frame % 500 >= 0 && d->st->count_frame % 500 <= 50)
			return (d->tex->player_down_eye_img);
		else
			return (d->tex->player_down_img);
	}
	if (d->map->direction == LEFT)
		return (d->tex->player_left_img);
	if (d->map->direction == RIGHT)
		return (d->tex->player_right_img);
	return (NULL);
}

void	*case_enemy(t_deta *d)
{
	if ((d->st->count_frame / 100) % 2 == 0)
		return (d->tex->enemy_move1_img);
	else
		return (d->tex->enemy_move2_img);
}

void	*case_trap(t_deta *d)
{
	if (d->st->count_frame % 1000 >= 0 && d->st->count_frame % 1000 <= 500)
		return (d->tex->trap2_img);
	else
		return (d->tex->trap1_img);
}

void	*case_item(t_deta *d)
{
	if (d->st->count_frame % 600 >= 0 && d->st->count_frame % 600 <= 15)
		return (d->tex->n->item2_img);
	if (d->st->count_frame % 600 >= 16 && d->st->count_frame % 600 <= 30)
		return (d->tex->n->item3_img);
	if (d->st->count_frame % 600 >= 31 && d->st->count_frame % 600 <= 45)
		return (d->tex->n->item4_img);
	if (d->st->count_frame % 600 >= 46 && d->st->count_frame % 600 <= 60)
		return (d->tex->n->item5_img);
	else
		return (d->tex->n->item1_img);
}
