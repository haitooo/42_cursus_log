/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:38:03 by tssaito           #+#    #+#             */
/*   Updated: 2025/08/24 11:13:44 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

// Holds the player's position and viewing direction
typedef struct s_player
{
	double				pos_x;
	double				pos_y;
	double				dir_x;
	double				dir_y;
	double				plane_x;
	double				plane_y;
}						t_player;

// Stores the map grid along with its width and height
typedef struct s_map
{
	char				**grid;
	int					width;
	int					height;
}						t_map;

// Collects texture paths and floor/ceiling colors
typedef struct s_config
{
	char				*north_texture_path;
	char				*south_texture_path;
	char				*west_texture_path;
	char				*east_texture_path;
	char				*door_texture_path;
	char				**animated_sprite_texture_paths;
	int					num_animated_sprite_frames;
	int					animated_sprite_paths_capacity;
	int					floor_color;
	int					ceiling_color;
}						t_config;

// Represents a sprite position
typedef struct s_sprite
{
	double				x;
	double				y;
}						t_sprite;

// MLX image pointer and pixel data
typedef struct s_img
{
	void				*img_ptr;
	char				*addr;
	int					bits_per_pixel;
	int					line_length;
	int					endian;
	int					width;
	int					height;
}						t_img;

// Variables used during ray casting
typedef struct s_ray
{
	double				camera_x;
	double				ray_dir_x;
	double				ray_dir_y;
	int					map_x;
	int					map_y;
	double				side_dist_x;
	double				side_dist_y;
	double				delta_dist_x;
	double				delta_dist_y;
	double				perp_wall_dist;
	int					step_x;
	int					step_y;
	int					hit;
	int					side;
}						t_ray;

// Door position and open state
typedef struct s_door
{
	int					x;
	int					y;
	int					is_open;
	double				open_progress;
	int					is_animating;
}						t_door;

// Data used for minimap drawing
typedef struct s_minimap
{
	int					player_x;
	int					player_y;
	double				dir_line_x;
	double				dir_line_y;
	int					i;
}						t_minimap;

// Central structure containing all game data
typedef struct s_game
{
	void				*mlx;
	void				*win;
	int					win_width;
	int					win_height;
	t_player			player;
	t_map				map;
	t_config			config;
	t_img				img;
	t_img				*textures;
	int					num_textures;
	t_door				*doors;
	int					num_doors;
	t_sprite			*sprites;
	int					num_sprites;
	int					first_mouse_move;
	double				*z_buffer;
	int					frame_count;
	int					show_minimap;
}						t_game;

// Projected wall height and draw range
typedef struct s_wall_projection
{
	int					line_height;
	int					draw_start;
	int					draw_end;
}						t_wall_projection;

// Texture number and wall hit location
typedef struct s_texture_info
{
	double				wall_x;
	int					tex_num;
	int					tex_x;
}						t_texture_info;

// Parameters for rendering a vertical wall strip
typedef struct s_strip_params
{
	int					x;
	int					line_height;
	int					draw_start;
	int					draw_end;
	int					tex_num;
	int					tex_x;
	double				open_progress;
}						t_strip_params;

// Screen coordinates and size for a sprite
typedef struct s_sprite_params
{
	double				sprite_x;
	double				sprite_y;
	double				inv_det;
	double				transform_x;
	double				transform_y;
	int					sprite_screen_x;
	int					sprite_height;
	int					draw_start_y;
	int					draw_end_y;
	int					sprite_width;
	int					draw_start_x;
	int					draw_end_x;
}						t_sprite_params;

typedef struct s_ray_render_params
{
	t_ray				ray;
	t_wall_projection	proj;
	t_texture_info		tex_info;
	t_strip_params		strip_params;
}						t_ray_render_params;

// Groups multiple structs for ray rendering
typedef struct s_sprite_calc_params
{
	double				sprite_x;
	double				sprite_y;
	double				inv_det;
	double				transform_x;
	double				transform_y;
}						t_sprite_calc_params;

// Values for calculating sprite projection
typedef struct s_vertical_strip_calc_params
{
	double				step;
	double				tex_pos;
}						t_vertical_strip_calc_params;

// Variables for sorting sprites by distance
typedef struct s_sort_sprites_params
{
	int					i;
	int					j;
	double				*dist;
}						t_sort_sprites_params;

// Temporary variables during sprite drawing
typedef struct s_sprite_strip_vars
{
	int					y;
	int					d;
	int					tex_x;
	int					tex_y;
	int					current_frame_tex_num;
}						t_sprite_strip_vars;

// Parameters for door checking
typedef struct s_door_check_params
{
	int					i;
}						t_door_check_params;

// Coordinates used to draw pixels per strip
typedef struct s_pixel_draw_info
{
	int					stripe;
	int					y;
	int					tex_x;
	int					tex_y;
	int					current_frame_tex_num;
}						t_pixel_draw_info;

// Info needed to compute texture coordinates
typedef struct s_tex_coord_calc_info
{
	int					stripe;
	int					*tex_x;
	int					*current_frame_tex_num;
}						t_tex_coord_calc_info;

// Holds one line of map data while parsing
typedef struct s_map_line_data
{
	char				*current_line;
	char				*trimmed_line;
	int					fd;
}						t_map_line_data;

// Stores individual RGB values
typedef struct s_rgb_values
{
	int					r;
	int					g;
	int					b;
}						t_rgb_values;

// Context for processing map lines
typedef struct s_map_line_proc
{
	t_list				*map_list;
	char				*current_line;
	int					fd;
	t_game				*game;

}						t_map_line_proc;

// Center position, size and color for drawing a star
typedef struct s_star_params
{
	int					center_x;
	int					center_y;
	int					size;
	int					color;
}						t_star_params;

#endif
