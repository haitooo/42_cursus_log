/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_left.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 15:03:35 by haito             #+#    #+#             */
/*   Updated: 2025/02/22 15:26:02 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	swap_left(t_deta *d)
{
	char	tmp;

	tmp = d->map->objs[d->map->y][d->map->x - 1];
	d->map->objs[d->map->y][d->map->x - 1]
		= d->map->objs[d->map->y][d->map->x];
	d->map->objs[d->map->y][d->map->x--] = tmp;
}

void	case_exit_left(t_deta *d)
{
	if (d->map->objs[d->map->y][d->map->x - 1] == 'E'
		&& d->map->num_c == d->st->count_got_item)
	{
		swap_left(d);
		ft_printf("Clear !!!!!!!!!\n");
		key_esc(d);
	}
}

void	key_left(t_deta *d)
{
	if (d->map->direction == LEFT)
	{
		if (d->map->objs[d->map->y][d->map->x - 1] != '1')
		{
			if (d->map->objs[d->map->y][d->map->x - 1] == 'C')
			{
				d->map->objs[d->map->y][d->map->x - 1] = '0';
				d->st->count_got_item++;
			}
			if (d->map->objs[d->map->y][d->map->x - 1] == 'E')
			{
				case_exit_left(d);
				return ;
			}
			swap_left(d);
			ft_printf("x:%d y:%d step %d\n", d->map->x,
				d->map->y, ++d->st->count_step);
		}
	}
	else
		d->map->direction = LEFT;
}
