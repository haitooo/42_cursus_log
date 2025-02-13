/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 14:15:39 by haito             #+#    #+#             */
/*   Updated: 2025/02/13 14:50:08 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int	start_game()
{
	mlx = mlx_init();
	if (!mlx)
		return (write(1, "Error\n", 6), 1);
}

int	main(int argc, char **argv)
{
	if (argc >= 3)
		return (1);
	check_map_error();
	start_game();
}
