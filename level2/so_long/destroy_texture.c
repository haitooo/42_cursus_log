/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy_texture.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 00:24:32 by haito             #+#    #+#             */
/*   Updated: 2025/02/28 08:03:32 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	destroy_imgs4(t_deta *deta)
{
	if (deta->tex->player_up_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->player_up_img);
	if (deta->tex->player_down_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->player_down_img);
	if (deta->tex->player_down_eye_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->player_down_eye_img);
	if (deta->tex->player_left_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->player_left_img);
	if (deta->tex->player_right_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->player_right_img);
	if (deta->tex->n->item1_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->n->item1_img);
	if (deta->tex->n->item2_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->n->item2_img);
	if (deta->tex->n->item3_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->n->item3_img);
	if (deta->tex->n->item4_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->n->item4_img);
	if (deta->tex->n->item5_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->n->item5_img);
}

void	destroy_imgs3(t_deta *deta)
{
	if (deta->tex->n->five_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->n->five_img);
	if (deta->tex->n->six_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->n->six_img);
	if (deta->tex->n->seven_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->n->seven_img);
	if (deta->tex->n->eight_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->n->eight_img);
	if (deta->tex->n->nine_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->n->nine_img);
	if (deta->tex->trap1_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->trap1_img);
	if (deta->tex->trap2_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->trap2_img);
	if (deta->tex->enemy_stop_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->enemy_stop_img);
	if (deta->tex->enemy_move1_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->enemy_move1_img);
	if (deta->tex->enemy_move2_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->enemy_move2_img);
	destroy_imgs4(deta);
}

void	destroy_imgs2(t_deta *deta)
{
	if (deta->tex->n->status_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->n->status_img);
	if (deta->tex->n->status1_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->n->status1_img);
	if (deta->tex->n->status2_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->n->status2_img);
	if (deta->tex->n->status3_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->n->status3_img);
	if (deta->tex->n->status4_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->n->status4_img);
	if (deta->tex->n->status5_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->n->status5_img);
	if (deta->tex->n->status6_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->n->status6_img);
	if (deta->tex->n->zero_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->n->zero_img);
	if (deta->tex->n->one_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->n->one_img);
	if (deta->tex->n->two_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->n->two_img);
	if (deta->tex->n->three_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->n->three_img);
	if (deta->tex->n->four_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->n->four_img);
	destroy_imgs3(deta);
}

void	destroy_imgs(t_deta *deta)
{
	if (deta->tex->back_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->back_img);
	if (deta->tex->exit_close_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->exit_close_img);
	if (deta->tex->exit_open_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->exit_open_img);
	if (deta->tex->wall_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->wall_img);
	if (deta->tex->n->par_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->n->par_img);
	if (deta->tex->heart_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->heart_img);
	if (deta->tex->heart_half_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->heart_half_img);
	if (deta->tex->heart_zero_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->heart_zero_img);
	destroy_imgs2(deta);
}
