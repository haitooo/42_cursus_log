#include "so_long.h"

void	import_heart(t_window *win, t_texture *tex)
{
	tex->heart_img = mlx_xpm_file_to_image(win->mlx, TEX_HEART,
			&tex->img_width, &tex->img_height);
	if (!tex->heart_img)
		return ;
	tex->heart_half_img = mlx_xpm_file_to_image(win->mlx, TEX_HEART_HALF,
			&tex->img_width, &tex->img_height);
	if (!tex->heart_half_img)
		return ;
	tex->heart_zero_img = mlx_xpm_file_to_image(win->mlx, TEX_HEART_ZERO,
			&tex->img_width, &tex->img_height);
	if (!tex->heart_zero_img)
		return ;
}

void	import_numbers2(t_window *win, t_texture *tex)
{
	tex->six_img = mlx_xpm_file_to_image(win->mlx, TEX_SIX,
			&tex->img_width, &tex->img_height);
	if (!tex->six_img)
		return ;
	tex->seven_img = mlx_xpm_file_to_image(win->mlx, TEX_SEVEN,
			&tex->img_width, &tex->img_height);
	if (!tex->seven_img)
		return ;
	tex->eight_img = mlx_xpm_file_to_image(win->mlx, TEX_EIGHT,
			&tex->img_width, &tex->img_height);
	if (!tex->eight_img)
		return ;
	tex->nine_img = mlx_xpm_file_to_image(win->mlx, TEX_NINE,
			&tex->img_width, &tex->img_height);
	if (!tex->nine_img)
		return ;
}

void	import_numbers(t_window *win, t_texture *tex)
{
	tex->zero_img = mlx_xpm_file_to_image(win->mlx, TEX_ZERO,
			&tex->img_width, &tex->img_height);
	if (!tex->zero_img)
		return ;
	tex->one_img = mlx_xpm_file_to_image(win->mlx, TEX_ONE,
			&tex->img_width, &tex->img_height);
	if (!tex->one_img)
		return ;
	tex->two_img = mlx_xpm_file_to_image(win->mlx, TEX_TWO,
			&tex->img_width, &tex->img_height);
	if (!tex->two_img)
		return ;
	tex->three_img = mlx_xpm_file_to_image(win->mlx, TEX_THREE,
			&tex->img_width, &tex->img_height);
	if (!tex->three_img)
		return ;
	tex->four_img = mlx_xpm_file_to_image(win->mlx, TEX_FOUR,
			&tex->img_width, &tex->img_height);
	if (!tex->four_img)
		return ;
	tex->five_img = mlx_xpm_file_to_image(win->mlx, TEX_FIVE,
			&tex->img_width, &tex->img_height);
	if (!tex->five_img)
		return ;
	import_numbers2(win, tex);
}
