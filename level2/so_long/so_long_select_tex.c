/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_select_tex.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 00:25:12 by haito             #+#    #+#             */
/*   Updated: 2025/02/28 08:03:49 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	*select_tex(t_deta *d, char c)
{
	if (c == '0')
		return (d->tex->back_img);
	if (c == '1')
		return (d->tex->wall_img);
	if (c == 'P')
		return (case_player(d));
	if (c == 'C')
		return (case_item(d));
	if (c == 'O')
		return (d->tex->exit_open_img);
	if (c == 'E')
		return (d->tex->exit_close_img);
	if (c == 'T')
		return (case_trap(d));
	if (c == 'M')
		return (d->tex->enemy_stop_img);
	if (c == 'D')
		return (case_enemy(d));
	return (NULL);
}

void	*select_status2_tex(t_texture *tex, int count)
{
	if (count == 0)
		return (tex->n->status4_img);
	if (count == 1)
		return (tex->n->status5_img);
	if (count == 2)
		return (tex->n->status6_img);
	else
		return (NULL);
}

void	*select_status_tex(t_texture *tex, int count)
{
	if (count == 0)
		return (tex->n->status1_img);
	if (count == 1)
		return (tex->n->status2_img);
	if (count == 2)
		return (tex->n->status3_img);
	else
		return (tex->n->status_img);
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
