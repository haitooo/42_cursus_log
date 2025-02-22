/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_up.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 15:03:35 by haito             #+#    #+#             */
/*   Updated: 2025/02/22 15:26:08 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	swap_up(t_deta *d)
{
	char	tmp;

	tmp = d->map->objs[d->map->y - 1][d->map->x];
	d->map->objs[d->map->y - 1][d->map->x]
		= d->map->objs[d->map->y][d->map->x];
	d->map->objs[d->map->y--][d->map->x] = tmp;
}

void	case_exit_up(t_deta *d)
{
	if (d->map->objs[d->map->y - 1][d->map->x] == 'E'
		&& d->map->num_c == d->st->count_got_item)
	{
		swap_up(d);
		ft_printf("Clear !!!!!!!!!\n");
		key_esc(d);
	}
}

void	key_up(t_deta *d)
{
	if (d->map->direction == UP)
	{
		if (d->map->objs[d->map->y - 1][d->map->x] != '1')
		{
			if (d->map->objs[d->map->y - 1][d->map->x] == 'C')
			{
				d->map->objs[d->map->y - 1][d->map->x] = '0';
				d->st->count_got_item++;
			}
			if (d->map->objs[d->map->y - 1][d->map->x] == 'E')
			{
				case_exit_up(d);
				return ;
			}
			swap_up(d);
			ft_printf("x:%d y:%d step %d\n", d->map->x,
				d->map->y, ++d->st->count_step);
		}
	}
	else
		d->map->direction = UP;
}
