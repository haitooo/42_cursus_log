/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   import_texture.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 00:24:53 by haito             #+#    #+#             */
/*   Updated: 2025/02/28 07:59:39 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int	import_status2(t_window *win, t_texture *tex)
{
	tex->n->status4_img = mlx_xpm_file_to_image(win->mlx, TEX_STATUS_FOUR,
			&tex->status_width, &tex->status_height);
	if (!tex->n->status4_img)
		return (-1);
	tex->n->status5_img = mlx_xpm_file_to_image(win->mlx, TEX_STATUS_FIVE,
			&tex->status_width, &tex->status_height);
	if (!tex->n->status5_img)
		return (-1);
	tex->n->status6_img = mlx_xpm_file_to_image(win->mlx, TEX_STATUS_SIX,
			&tex->status_width, &tex->status_height);
	if (!tex->n->status6_img)
		return (-1);
	tex->n->par_img = mlx_xpm_file_to_image(win->mlx, TEX_PAR,
			&tex->status_width, &tex->status_height);
	if (!tex->n->par_img)
		return (-1);
	return (0);
}

int	import_status(t_window *win, t_texture *tex)
{
	tex->n->status_img = mlx_xpm_file_to_image(win->mlx, TEX_STATUS_BACK,
			&tex->status_width, &tex->status_height);
	if (!tex->n->status_img)
		return (-1);
	tex->n->status1_img = mlx_xpm_file_to_image(win->mlx, TEX_STATUS_ONE,
			&tex->status_width, &tex->status_height);
	if (!tex->n->status1_img)
		return (-1);
	tex->n->status2_img = mlx_xpm_file_to_image(win->mlx, TEX_STATUS_TWO,
			&tex->status_width, &tex->status_height);
	if (!tex->n->status2_img)
		return (-1);
	tex->n->status3_img = mlx_xpm_file_to_image(win->mlx, TEX_STATUS_THREE,
			&tex->status_width, &tex->status_height);
	if (!tex->n->status3_img)
		return (-1);
	return (import_status2(win, tex));
}

int	import_player(t_window *win, t_texture *tex)
{
	tex->player_up_img = mlx_xpm_file_to_image(win->mlx, TEX_PLAYER_UP,
			&tex->img_width, &tex->img_height);
	if (!tex->player_up_img)
		return (-1);
	tex->player_down_img = mlx_xpm_file_to_image(win->mlx, TEX_PLAYER_DOWN,
			&tex->img_width, &tex->img_height);
	if (!tex->player_down_img)
		return (-1);
	tex->player_down_eye_img = mlx_xpm_file_to_image(win->mlx, TEX_PLAYER_EYE,
			&tex->img_width, &tex->img_height);
	if (!tex->player_down_eye_img)
		return (-1);
	tex->player_left_img = mlx_xpm_file_to_image(win->mlx, TEX_PLAYER_LEFT,
			&tex->img_width, &tex->img_height);
	if (!tex->player_left_img)
		return (-1);
	tex->player_right_img = mlx_xpm_file_to_image(win->mlx, TEX_PLAYER_RIGHT,
			&tex->img_width, &tex->img_height);
	if (!tex->player_right_img)
		return (-1);
	return (0);
}

int	import_tiles(t_window *win, t_texture *tex)
{
	tex->exit_close_img = mlx_xpm_file_to_image(win->mlx, TEX_EXIT_CLOSE,
			&tex->img_width, &tex->img_height);
	if (!tex->exit_close_img)
		return (-1);
	tex->exit_open_img = mlx_xpm_file_to_image(win->mlx, TEX_EXIT_OPEN,
			&tex->img_width, &tex->img_height);
	if (!tex->exit_open_img)
		return (-1);
	tex->wall_img = mlx_xpm_file_to_image(win->mlx, TEX_WALL,
			&tex->img_width, &tex->img_height);
	if (!tex->wall_img)
		return (-1);
	tex->back_img = mlx_xpm_file_to_image(win->mlx, TEX_BACK,
			&tex->img_width, &tex->img_height);
	if (!tex->back_img)
		return (-1);
	return (0);
}

void	import_texture(t_deta *d)
{
	if (import_tiles(d->win, d->tex) == -1)
		error_import(d);
	if (import_player(d->win, d->tex) == -1)
		error_import(d);
	if (import_item(d->win, d->tex) == -1)
		error_import(d);
	if (import_status(d->win, d->tex) == -1)
		error_import(d);
	if (import_heart(d->win, d->tex) == -1)
		error_import(d);
	if (import_numbers(d->win, d->tex) == -1)
		error_import(d);
	if (import_enemy(d->win, d->tex) == -1)
		error_import(d);
}
