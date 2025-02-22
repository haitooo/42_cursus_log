/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_down.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 15:03:35 by haito             #+#    #+#             */
/*   Updated: 2025/02/22 15:27:40 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	swap_down(t_deta *d)
{
	char	tmp;

	tmp = d->map->objs[d->map->y + 1][d->map->x];
	d->map->objs[d->map->y + 1][d->map->x]
		= d->map->objs[d->map->y][d->map->x];
	d->map->objs[d->map->y++][d->map->x] = tmp;
}

void	case_exit_down(t_deta *d)
{
	if (d->map->objs[d->map->y + 1][d->map->x] == 'E'
		&& d->map->num_c == d->st->count_got_item)
	{
		swap_down(d);
		ft_printf("Clear !!!!!!!!!\n");
		key_esc(d);
	}
}

void	key_down(t_deta *d)
{
	if (d->map->direction == DOWN)
	{
		if (d->map->objs[d->map->y + 1][d->map->x] != '1')
		{
			if (d->map->objs[d->map->y + 1][d->map->x] == 'C')
			{
				d->map->objs[d->map->y + 1][d->map->x] = '0';
				d->st->count_got_item++;
			}
			if (d->map->objs[d->map->y + 1][d->map->x] == 'E')
			{
				case_exit_down(d);
				return ;
			}
			swap_down(d);
			ft_printf("x:%d y:%d step %d\n", d->map->x,
				d->map->y, ++d->st->count_step);
		}
	}
	else
		d->map->direction = DOWN;
}
