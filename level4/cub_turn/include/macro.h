/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   macro.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:38:01 by tssaito           #+#    #+#             */
/*   Updated: 2025/09/01 13:22:39 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MACRO_H
# define MACRO_H

// Constants
# define WIN_WIDTH 1600
# define WIN_HEIGHT 1200
# define MOVE_SPEED 0.1
# define ROT_SPEED 0.05
# define MOUSE_SENSITIVITY 0.0002

// Key codes
# define KEY_W 119
# define KEY_A 97
# define KEY_S 115
# define KEY_D 100
# define KEY_Z 122
# define KEY_Q 113
# define KEY_E 101
# define KEY_M 109
# define KEY_ESC 65307
# define KEY_LEFT 65361
# define KEY_RIGHT 65363
# define KEY_UP 65362
# define KEY_DOWN 65364
# define KEY_SPACE 32

// Minimap constants
# define MINIMAP_SCALE 16
# define MINIMAP_WALL_COLOR 0x00808080
# define MINIMAP_FLOOR_COLOR 0x0000008B
# define MINIMAP_CLOSED_DOOR_COLOR 0x00404040
# define MINIMAP_OPEN_DOOR_COLOR 0x00ADD8E6
# define MINIMAP_PLAYER_COLOR 0x00FF0000
# define MINIMAP_DIR_COLOR 0x00FFFF00

#endif
