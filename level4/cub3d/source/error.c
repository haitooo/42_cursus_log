/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:26:21 by tssaito           #+#    #+#             */
/*   Updated: 2025/08/24 13:18:07 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Prints an error message, frees game resources, and exits the program.
void	exit_error(char *message, t_game *game)
{
	printf("Error\n%s\n", message);
	if (game)
		free_game(game);
	exit(EXIT_FAILURE);
}

// Frees all allocated resources for the game.
void	free_game(t_game *game)
{
	free_game_map_config(game);
	free_game_images(game);
	free_game_mlx(game);
	free(game->mlx);
	game->mlx = NULL;
	free(game);
	game = NULL;
}
