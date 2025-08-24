CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCS_DIR = source
OBJS_DIR = object

SRCS = main.c \
	init.c init_game_data.c init_player_direction.c init_textures.c \
	error.c error_free_utils.c \
	hooks.c hook_wrappers.c \
	parse_main.c \
	parse_map_data.c parse_map_validation.c parse_entities.c \
	parse_elements.c  parse_animated_elements.c \
	parse_color_utils.c parse_door_utils.c parse_sprite_utils.c \
	parse_map_grid_utils.c parse_map_validation_utils.c parse_map_utils.c \
	render_main.c \
	render_sprite.c render_sprite_calc_utils.c render_sprite_draw_utils.c render_sprite_sort_utils.c \
	render_projection.c render_ray_casting.c render_texture.c \
	render_utils.c render_texture_utils.c render_animation_utils.c \
	minimap_draw_utils.c minimap.c \
	move.c move_player.c\
	rotate_player.c \
	utils.c

            
OBJS = $(addprefix $(OBJS_DIR)/, $(SRCS:.c=.o))

NAME = cub3D

MLX_DIR = minilibx
MLX_LIB = $(MLX_DIR)/libmlx.a
LIBFT_DIR = libft
LIBFT_LIB = $(LIBFT_DIR)/libft.a

INCLUDES = -Iinclude -I$(MLX_DIR) -I$(LIBFT_DIR)/inc
LIBS = -L$(MLX_DIR) -lmlx -lXext -lX11 -lm -L$(LIBFT_DIR) -lft

all: $(NAME)

bonus: $(NAME)

$(NAME): $(OBJS) $(MLX_LIB) $(LIBFT_LIB)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(LIBS) -o $(NAME)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@mkdir -p $(OBJS_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(MLX_LIB):
	@$(MAKE) -C $(MLX_DIR) CC=clang

$(LIBFT_LIB):
	@$(MAKE) -C $(LIBFT_DIR)

clean:
	@$(MAKE) -C $(MLX_DIR) clean
	@$(MAKE) -C $(LIBFT_DIR) clean
	@rm -rf $(OBJS_DIR)

fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@rm -f $(NAME) $(MLX_LIB)

re: fclean all

.PHONY: all clean fclean re
