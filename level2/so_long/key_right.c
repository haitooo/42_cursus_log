/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_right.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 15:03:35 by haito             #+#    #+#             */
/*   Updated: 2025/02/22 15:23:11 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	swap_right(t_deta *d)
{
	char	tmp;

	tmp = d->map->objs[d->map->y][d->map->x + 1];
	d->map->objs[d->map->y][d->map->x + 1]
		= d->map->objs[d->map->y][d->map->x];
	d->map->objs[d->map->y][d->map->x++] = tmp;
}

void	case_exit_right(t_deta *d)
{
	if (d->map->objs[d->map->y][d->map->x + 1] == 'E'
		&& d->map->num_c == d->st->count_got_item)
	{
		swap_right(d);
		ft_printf("Clear !!!!!!!!!\n");
		key_esc(d);
	}
}

void	key_right(t_deta *d)
{
	if (d->map->direction == RIGHT)
	{
		if (d->map->objs[d->map->y][d->map->x + 1] != '1')
		{
			if (d->map->objs[d->map->y][d->map->x + 1] == 'C')
			{
				d->map->objs[d->map->y][d->map->x + 1] = '0';
				d->st->count_got_item++;
				if (d->st->count_got_item % 10 == 0 && d->st->hp < 10)
					d->st->hp++;
			}
			if (d->map->objs[d->map->y][d->map->x + 1] == 'E')
			{
				case_exit_right(d);
				return ;
			}
			swap_right(d);
			ft_printf("x:%d y:%d step %d\n", d->map->x,
				d->map->y, ++d->st->count_step);
		}
	}
	else
		d->map->direction = RIGHT;
}
