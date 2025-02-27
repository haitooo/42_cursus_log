/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   import_texture2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 00:24:57 by haito             #+#    #+#             */
/*   Updated: 2025/02/28 08:02:26 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int	import_enemy(t_window *win, t_texture *tex)
{
	tex->trap1_img = mlx_xpm_file_to_image(win->mlx, TEX_TRAP,
			&tex->img_width, &tex->img_height);
	if (!tex->trap1_img)
		return (-1);
	tex->trap2_img = mlx_xpm_file_to_image(win->mlx, TEX_TRAP_,
			&tex->img_width, &tex->img_height);
	if (!tex->trap2_img)
		return (-1);
	tex->enemy_stop_img = mlx_xpm_file_to_image(win->mlx, TEX_ENEMY_STOP,
			&tex->img_width, &tex->img_height);
	if (!tex->enemy_stop_img)
		return (-1);
	tex->enemy_move1_img = mlx_xpm_file_to_image(win->mlx, TEX_ENEMY_MOVE,
			&tex->img_width, &tex->img_height);
	if (!tex->enemy_move1_img)
		return (-1);
	tex->enemy_move2_img = mlx_xpm_file_to_image(win->mlx, TEX_ENEMY_MOVE_,
			&tex->img_width, &tex->img_height);
	if (!tex->enemy_move2_img)
		return (-1);
	return (0);
}

int	import_heart(t_window *win, t_texture *tex)
{
	tex->heart_img = mlx_xpm_file_to_image(win->mlx, TEX_HEART,
			&tex->img_width, &tex->img_height);
	if (!tex->heart_img)
		return (-1);
	tex->heart_half_img = mlx_xpm_file_to_image(win->mlx, TEX_HEART_HALF,
			&tex->img_width, &tex->img_height);
	if (!tex->heart_half_img)
		return (-1);
	tex->heart_zero_img = mlx_xpm_file_to_image(win->mlx, TEX_HEART_ZERO,
			&tex->img_width, &tex->img_height);
	if (!tex->heart_zero_img)
		return (-1);
	return (0);
}

int	import_numbers2(t_window *win, t_texture *tex)
{
	tex->n->six_img = mlx_xpm_file_to_image(win->mlx, TEX_SIX,
			&tex->img_width, &tex->img_height);
	if (!tex->n->six_img)
		return (-1);
	tex->n->seven_img = mlx_xpm_file_to_image(win->mlx, TEX_SEVEN,
			&tex->img_width, &tex->img_height);
	if (!tex->n->seven_img)
		return (-1);
	tex->n->eight_img = mlx_xpm_file_to_image(win->mlx, TEX_EIGHT,
			&tex->img_width, &tex->img_height);
	if (!tex->n->eight_img)
		return (-1);
	tex->n->nine_img = mlx_xpm_file_to_image(win->mlx, TEX_NINE,
			&tex->img_width, &tex->img_height);
	if (!tex->n->nine_img)
		return (-1);
	return (0);
}

int	import_numbers(t_window *win, t_texture *tex)
{
	tex->n->zero_img = mlx_xpm_file_to_image(win->mlx, TEX_ZERO,
			&tex->img_width, &tex->img_height);
	if (!tex->n->zero_img)
		return (-1);
	tex->n->one_img = mlx_xpm_file_to_image(win->mlx, TEX_ONE,
			&tex->img_width, &tex->img_height);
	if (!tex->n->one_img)
		return (-1);
	tex->n->two_img = mlx_xpm_file_to_image(win->mlx, TEX_TWO,
			&tex->img_width, &tex->img_height);
	if (!tex->n->two_img)
		return (-1);
	tex->n->three_img = mlx_xpm_file_to_image(win->mlx, TEX_THREE,
			&tex->img_width, &tex->img_height);
	if (!tex->n->three_img)
		return (-1);
	tex->n->four_img = mlx_xpm_file_to_image(win->mlx, TEX_FOUR,
			&tex->img_width, &tex->img_height);
	if (!tex->n->four_img)
		return (-1);
	tex->n->five_img = mlx_xpm_file_to_image(win->mlx, TEX_FIVE,
			&tex->img_width, &tex->img_height);
	if (!tex->n->five_img)
		return (-1);
	return (import_numbers2(win, tex));
}

int	import_item(t_window *win, t_texture *tex)
{
	tex->n->item1_img = mlx_xpm_file_to_image(win->mlx, TEX_ITEM,
			&tex->img_width, &tex->img_height);
	if (!tex->n->item1_img)
		return (-1);
	tex->n->item2_img = mlx_xpm_file_to_image(win->mlx, TEX_ITEM_,
			&tex->img_width, &tex->img_height);
	if (!tex->n->item2_img)
		return (-1);
	tex->n->item3_img = mlx_xpm_file_to_image(win->mlx, TEX_ITEM__,
			&tex->img_width, &tex->img_height);
	if (!tex->n->item3_img)
		return (-1);
	tex->n->item4_img = mlx_xpm_file_to_image(win->mlx, TEX_ITEM___,
			&tex->img_width, &tex->img_height);
	if (!tex->n->item4_img)
		return (-1);
	tex->n->item5_img = mlx_xpm_file_to_image(win->mlx, TEX_ITEM____,
			&tex->img_width, &tex->img_height);
	if (!tex->n->item5_img)
		return (-1);
	return (0);
}
