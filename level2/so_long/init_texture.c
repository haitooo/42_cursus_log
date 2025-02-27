/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_texture.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 06:34:17 by haito             #+#    #+#             */
/*   Updated: 2025/02/28 07:58:53 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	init_texture2(t_texture *t)
{
	t->n->eight_img = NULL;
	t->n->five_img = NULL;
	t->n->four_img = NULL;
	t->n->nine_img = NULL;
	t->n->one_img = NULL;
	t->n->par_img = NULL;
	t->n->seven_img = NULL;
	t->n->six_img = NULL;
	t->n->status1_img = NULL;
	t->n->status2_img = NULL;
	t->n->status3_img = NULL;
	t->n->status4_img = NULL;
	t->n->status5_img = NULL;
	t->n->status6_img = NULL;
	t->n->status_img = NULL;
	t->n->three_img = NULL;
	t->n->two_img = NULL;
	t->n->zero_img = NULL;
	t->n->item1_img = NULL;
	t->n->item2_img = NULL;
	t->n->item3_img = NULL;
	t->n->item4_img = NULL;
	t->n->item5_img = NULL;
}

void	init_texture(t_texture *t)
{
	t->back_img = NULL;
	t->enemy_move1_img = NULL;
	t->enemy_move2_img = NULL;
	t->enemy_stop_img = NULL;
	t->exit_close_img = NULL;
	t->exit_open_img = NULL;
	t->heart_half_img = NULL;
	t->heart_img = NULL;
	t->heart_zero_img = NULL;
	t->player_down_eye_img = NULL;
	t->player_down_img = NULL;
	t->player_left_img = NULL;
	t->player_right_img = NULL;
	t->player_up_img = NULL;
	t->trap1_img = NULL;
	t->trap2_img = NULL;
	t->wall_img = NULL;
	init_texture2(t);
}
