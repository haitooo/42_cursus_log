#include "so_long.h"

void	destroy_imgs(t_deta *deta)
{
	if (deta->tex->back_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->back_img);
	if (deta->tex->player_up_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->player_up_img);
	if (deta->tex->player_down_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->player_down_img);
	if (deta->tex->player_left_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->player_left_img);
	if (deta->tex->player_right_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->player_right_img);
	if (deta->tex->item_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->item_img);
	if (deta->tex->exit_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->exit_img);
	if (deta->tex->wall_img)
		mlx_destroy_image(deta->win->mlx, deta->tex->wall_img);
}
