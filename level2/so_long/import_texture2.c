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
	tex->n->six_img = mlx_xpm_file_to_image(win->mlx, TEX_SIX,
			&tex->img_width, &tex->img_height);
	if (!tex->n->six_img)
		return ;
	tex->n->seven_img = mlx_xpm_file_to_image(win->mlx, TEX_SEVEN,
			&tex->img_width, &tex->img_height);
	if (!tex->n->seven_img)
		return ;
	tex->n->eight_img = mlx_xpm_file_to_image(win->mlx, TEX_EIGHT,
			&tex->img_width, &tex->img_height);
	if (!tex->n->eight_img)
		return ;
	tex->n->nine_img = mlx_xpm_file_to_image(win->mlx, TEX_NINE,
			&tex->img_width, &tex->img_height);
	if (!tex->n->nine_img)
		return ;
}

void	import_numbers(t_window *win, t_texture *tex)
{
	tex->n->zero_img = mlx_xpm_file_to_image(win->mlx, TEX_ZERO,
			&tex->img_width, &tex->img_height);
	if (!tex->n->zero_img)
		return ;
	tex->n->one_img = mlx_xpm_file_to_image(win->mlx, TEX_ONE,
			&tex->img_width, &tex->img_height);
	if (!tex->n->one_img)
		return ;
	tex->n->two_img = mlx_xpm_file_to_image(win->mlx, TEX_TWO,
			&tex->img_width, &tex->img_height);
	if (!tex->n->two_img)
		return ;
	tex->n->three_img = mlx_xpm_file_to_image(win->mlx, TEX_THREE,
			&tex->img_width, &tex->img_height);
	if (!tex->n->three_img)
		return ;
	tex->n->four_img = mlx_xpm_file_to_image(win->mlx, TEX_FOUR,
			&tex->img_width, &tex->img_height);
	if (!tex->n->four_img)
		return ;
	tex->n->five_img = mlx_xpm_file_to_image(win->mlx, TEX_FIVE,
			&tex->img_width, &tex->img_height);
	if (!tex->n->five_img)
		return ;
	import_numbers2(win, tex);
}
