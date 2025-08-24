/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:37:47 by tssaito           #+#    #+#             */
/*   Updated: 2025/08/24 11:13:38 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "libft.h"
# include "macro.h"
# include "mlx.h"
# include "struct.h"
# include <fcntl.h>
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>

// init.c
void			init_game(t_game *game);
void			init_player(t_game *game, char orientation, int x, int y);

// init_game_data.c
void			init_game_mlx_and_img(t_game *game);
void			init_game_player_data(t_game *game);
void			init_game_map_data(t_game *game);
void			init_game_config_data(t_game *game);
void			init_game_textures_data(t_game *game);

// init_player_direction.c
void			set_player_direction_north(t_player *player);
void			set_player_direction_south(t_player *player);
void			set_player_direction_east(t_player *player);
void			set_player_direction_west(t_player *player);

// init_textures.c
void			load_single_texture(t_game *game, t_img *texture, char *path);
void			load_textures(t_game *game);

// parse_main.c
void			parse_cub_file(char *file_path, t_game *game);
int				process_cub_line(char *line, t_game *game, int *elements_count,
					int fd);

// parse_elements.c
int				handle_texture_line(char *line, t_game *game);
int				handle_color_line(char *line, t_game *game);
void			validate_color_values(int r, int g, int b, t_game *game);
void			parse_texture_path(char *line, char **path, t_game *game);
void			parse_color(char *line, int *color, t_game *game);

// parse_animated_elements.c
void			get_animated_sprite_frame_num_and_path(char *line, t_game *game,
					char ***parts, int *frame_num);
void			reallocate_animated_sprite_paths(t_game *game, int frame_num);
void			parse_animated_sprite_texture_path(char *line, t_game *game);

// parse_color_utils.c
void			get_rgb_strings(char *line, t_game *game, char ***rgb_str);
void			convert_and_validate_rgb(char **rgb_str, t_rgb_values *rgb,
					t_game *game);

// parse_map_data.c
int				handle_map_line(int fd, char *line, t_game *game);
t_list			*read_map_lines_to_list(int fd, char *line, t_game *game);
void			init_map_grid(t_game *game);
void			populate_map_grid(t_list *map_list, t_game *game);
void			parse_map(int fd, char *line, t_game *game);

// parse_map_utils.c
void			add_map_line_to_list(t_list **map_list, char *line,
					t_game *game);
char			*trim_and_check_empty_line(t_map_line_proc *proc,
					char **trimmed_line);
void			add_line_to_map_list(t_list **map_list, char *current_line,
					t_game *game);
char			*process_single_map_line(t_map_line_proc *proc);
void			process_map_lines_loop(t_map_line_proc *proc);

// parse_map_grid_utils.c
void			allocate_and_copy_row(t_game *game, t_list *map_list, int i);
void			process_map_list_node(t_game *game, t_list **map_list, int *i);

// parse_map_validation.c
void			validate_map(t_game *game);
void			validate_map_characters(t_game *game, int *player_count);
void			check_map_walls(t_game *game);
void			process_map_character(t_game *game, int y, int x,
					int *player_count);
void			check_wall_at_position(t_game *game, int y, int x);
void			validate_texture_requirements(t_game *game);
void			validate_animated_sprite_sequence(t_game *game);

// parse_map_validation_utils.c
void			iterate_map_and_process_characters(t_game *game,
					int *player_count);
void			iterate_map_and_check_walls(t_game *game);

// parse_entities.c
void			process_doors(t_game *game);
void			process_sprites(t_game *game);
int				count_doors_in_map(t_game *game);
void			init_door_data(t_game *game);
int				count_sprites_in_map(t_game *game);

// parse_door_utils.c
void			init_single_door_data(t_game *game, int i, int j,
					int *door_idx);

// parse_sprite_utils.c
void			init_single_sprite_data(t_game *game, int i, int j,
					int *sprite_idx);
void			init_sprite_data(t_game *game);

// render_main.c
int				render_frame(t_game *game);
void			clear_background(t_game *game);
void			process_wall_rendering_loop_details(t_game *game,
					t_ray_render_params *params, int x);
void			init_ray_render_params_door_check(t_game *game,
					t_ray_render_params *params);
void			update_door_animation(t_game *game);

// render_animation_utils.c
void			update_single_door_animation(t_game *game, int i);

// render_ray_casting.c
void			calculate_ray_params(t_game *game, t_ray *ray, int x);
void			perform_dda(t_game *game, t_ray *ray);
void			calculate_wall_projection(t_game *game, t_ray *ray,
					t_wall_projection *proj);

// render_texture.c
void			get_texture_params(t_game *game, t_ray *ray,
					t_texture_info *tex_info);
void			draw_vertical_strip(t_game *game, t_strip_params *params);
void			set_texture_number(t_game *game, t_ray *ray,
					t_texture_info *tex_info);
void			adjust_texture_x(t_game *game, t_ray *ray,
					t_texture_info *tex_info);
void			adjust_door_texture_x(t_game *game, t_strip_params *params);

// render_texture_utils.c
unsigned int	get_texture_pixel_color(t_game *game, t_strip_params *params,
					int tex_y);
void			calculate_strip_texture_position(t_game *game,
					t_strip_params *params,
					t_vertical_strip_calc_params *calc_params);

// render_sprite.c
void			draw_sprites(t_game *game);
void			sort_sprites(t_game *game);
void			calculate_sprite_params(t_game *game, t_sprite *sprite,
					t_sprite_params *params);

// render_sprite_sort_utils.c
void			double_swap(double *a, double *b);
void			calculate_sprite_distances(t_game *game,
					t_sort_sprites_params *params);
void			sort_sprite_array(t_game *game, t_sort_sprites_params *params);

// render_sprite_calc_utils.c
void			calculate_sprite_transform(t_game *game, t_sprite *sprite,
					t_sprite_calc_params *calc_params);
void			calculate_sprite_screen_and_dimensions(t_game *game,
					t_sprite_calc_params *calc_params, t_sprite_params *params);
void			calculate_sprite_tex_coords(t_game *game,
					t_tex_coord_calc_info *info, t_sprite_params *params);

// render_sprite_draw_utils.c
void			draw_sprite_pixel(t_game *game, t_pixel_draw_info *info);
void			draw_sprite_strip_loop_body(t_game *game, int stripe,
					t_sprite_params *params, t_sprite_strip_vars *vars);
void			draw_sprite_vertical_strip(t_game *game, int stripe,
					t_sprite_params *params);

// render_utils.c
void			my_mlx_pixel_put(t_img *img, int x, int y, int color);

// minimap.c
void			draw_minimap(t_game *game);

// minimap_draw_utils.c
void			put_pixel_to_img(t_img *img, int x, int y, int color);
void			draw_square(t_game *game, int map_x, int map_y, int color);
void			draw_player_circle(t_game *game, int center_x, int center_y,
					int radius);
void			draw_star(t_game *game, t_star_params *params);

// hooks.c
int				key_press_hook(int keycode, t_game *game);
int				close_window_hook(t_game *game);
int				mouse_move_hook(int x, int y, t_game *game);

// hook_wrappers.c
int				key_press_wrapper(int keycode, void *param);
int				close_window_wrapper(void *param);
int				render_frame_wrapper(void *param);

// move.c
void			handle_door_interaction(t_game *game);
int				is_door_open(t_game *game, int x, int y);
void			check_and_close_doors(t_game *game);

// move_player.c
void			move_forward(t_game *game);
void			move_backward(t_game *game);
void			move_left(t_game *game);
void			move_right(t_game *game);

// rotate_player.c
void			rotate_left(t_game *game);
void			rotate_right(t_game *game);
void			rotate_player_direction(t_game *game, double rot_speed);
void			rotate_player_plane(t_game *game, double rot_speed);

// error.c
void			exit_error(char *message, t_game *game);
void			check_file_extension(const char *filename);
void			free_game(t_game *game);

// error_free_utils.c
void			free_game_map_config(t_game *game);
void			free_config_paths(t_game *game);
void			free_animated_sprites(t_game *game);
void			free_game_images(t_game *game);
void			free_game_mlx(t_game *game);

// utils.c
void			ft_free_split(char **arr);

#endif
