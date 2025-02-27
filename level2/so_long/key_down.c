/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_down.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 15:03:35 by haito             #+#    #+#             */
/*   Updated: 2025/02/28 07:27:30 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	on_trap_down(t_deta *d)
{
	d->map->objs[d->map->y + 1][d->map->x] = '0';
	d->st->hp -= 2;
	if (d->st->hp <= 0)
	{
		ft_printf("-----Game over-----\n");
		key_esc(d);
	}
}

void	swap_down(t_deta *d)
{
	char	tmp;

	if (d->map->objs[d->map->y + 1][d->map->x] == 'T')
		on_trap_down(d);
	if (d->map->objs[d->map->y + 1][d->map->x] == 'M'
		|| d->map->objs[d->map->y + 1][d->map->x] == 'D')
	{
		if (--d->st->hp <= 0)
		{
			ft_printf("-----Game over-----\n");
			key_esc(d);
		}
		if (d->map->objs[d->map->y + 1][d->map->x] == 'M'
			|| d->map->objs[d->map->y + 1][d->map->x] == 'D')
			return ;
	}
	tmp = d->map->objs[d->map->y + 1][d->map->x];
	d->map->objs[d->map->y + 1][d->map->x] = d->map->objs[d->map->y][d->map->x];
	d->map->objs[d->map->y++][d->map->x] = tmp;
}

void	case_exit_down(t_deta *d)
{
	if (d->map->objs[d->map->y + 1][d->map->x] == 'O')
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
		check_patrol(d);
		if (d->map->objs[d->map->y + 1][d->map->x] != '1')
		{
			if (d->map->objs[d->map->y + 1][d->map->x] == 'C')
			{
				d->map->objs[d->map->y + 1][d->map->x] = '0';
				if (++d->st->count_got_item % 10 == 0 && d->st->hp < 10)
					d->st->hp++;
				if (d->map->num_c == d->st->count_got_item)
					d->map->objs[d->map->exit_y][d->map->exit_x] = 'O';
			}
			if (d->map->objs[d->map->y + 1][d->map->x] == 'O'
				|| d->map->objs[d->map->y + 1][d->map->x] == 'E')
			{
				case_exit_down(d);
				return ;
			}
			swap_down(d);
			ft_printf("step %d\n", ++d->st->count_step);
		}
	}
	else
		d->map->direction = DOWN;
}
