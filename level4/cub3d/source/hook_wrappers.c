/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook_wrappers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:26:24 by tssaito           #+#    #+#             */
/*   Updated: 2025/08/24 11:39:47 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Wrapper function for the key press hook to match MLX's required signature.
int	key_press_wrapper(int keycode, void *param)
{
	t_game	*game_ptr;

	game_ptr = (t_game *)param;
	return (key_press_hook(keycode, game_ptr));
}

// Wrapper function for the window close hook.
int	close_window_wrapper(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	return (close_window_hook(game));
}

// Wrapper function for the main rendering loop hook.
int	render_frame_wrapper(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	return (render_frame(game));
}
