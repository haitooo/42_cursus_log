#include "so_long.h"

void	import_status2(t_window *win, t_texture *tex)
{
	tex->status4_img = mlx_xpm_file_to_image(win->mlx, TEX_STATUS_FOUR,
			&tex->status_width, &tex->status_height);
	if (!tex->status4_img)
		return ;
	tex->status5_img = mlx_xpm_file_to_image(win->mlx, TEX_STATUS_FIVE,
			&tex->status_width, &tex->status_height);
	if (!tex->status5_img)
		return ;
	tex->status6_img = mlx_xpm_file_to_image(win->mlx, TEX_STATUS_SIX,
			&tex->status_width, &tex->status_height);
	if (!tex->status6_img)
		return ;
	tex->par_img = mlx_xpm_file_to_image(win->mlx, TEX_PAR,
			&tex->status_width, &tex->status_height);
	if (!tex->par_img)
		return ;
}

void	import_status(t_window *win, t_texture *tex)
{
	tex->status_img = mlx_xpm_file_to_image(win->mlx, TEX_STATUS_BACK,
			&tex->status_width, &tex->status_height);
	if (!tex->status_img)
		return ;
	tex->status1_img = mlx_xpm_file_to_image(win->mlx, TEX_STATUS_ONE,
			&tex->status_width, &tex->status_height);
	if (!tex->status1_img)
		return ;
	tex->status2_img = mlx_xpm_file_to_image(win->mlx, TEX_STATUS_TWO,
			&tex->status_width, &tex->status_height);
	if (!tex->status2_img)
		return ;
	tex->status3_img = mlx_xpm_file_to_image(win->mlx, TEX_STATUS_THREE,
			&tex->status_width, &tex->status_height);
	if (!tex->status3_img)
		return ;
	import_status2(win, tex);
}

void	import_player(t_window *win, t_texture *tex)
{
	tex->player_up_img = mlx_xpm_file_to_image(win->mlx, TEX_PLAYER_UP,
			&tex->img_width, &tex->img_height);
	if (!tex->player_up_img)
		return ;
	tex->player_down_img = mlx_xpm_file_to_image(win->mlx, TEX_PLAYER_DOWN,
			&tex->img_width, &tex->img_height);
	if (!tex->player_down_img)
		return ;
	tex->player_left_img = mlx_xpm_file_to_image(win->mlx, TEX_PLAYER_LEFT,
			&tex->img_width, &tex->img_height);
	if (!tex->player_left_img)
		return ;
	tex->player_right_img = mlx_xpm_file_to_image(win->mlx, TEX_PLAYER_RIGHT,
			&tex->img_width, &tex->img_height);
	if (!tex->player_right_img)
		return ;
}

void	import_tiles(t_window *win, t_texture *tex)
{
	tex->exit_img = mlx_xpm_file_to_image(win->mlx, TEX_EXIT,
			&tex->img_width, &tex->img_height);
	if (!tex->exit_img)
		return ;
	tex->item_img = mlx_xpm_file_to_image(win->mlx, TEX_ITEM,
			&tex->img_width, &tex->img_height);
	if (!tex->item_img)
		return ;
	tex->wall_img = mlx_xpm_file_to_image(win->mlx, TEX_WALL,
			&tex->img_width, &tex->img_height);
	if (!tex->wall_img)
		return ;
	tex->back_img = mlx_xpm_file_to_image(win->mlx, TEX_BACK,
			&tex->img_width, &tex->img_height);
	if (!tex->back_img)
		return ;
}

void	import_texture(t_window *win, t_texture *tex)
{
	import_tiles(win, tex);
	import_player(win, tex);
	import_status(win, tex);
	import_heart(win, tex);
	import_numbers(win, tex);
}
