# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: haito <haito@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/13 14:15:47 by haito             #+#    #+#              #
#    Updated: 2025/02/28 06:42:13 by haito            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = so_long
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCS = 	so_long_main.c \
		so_long_utils.c \
		so_long_draw.c \
		so_long_draw2.c \
		so_long_select_tex.c \
		so_long_select_tex2.c \
		so_long_free.c \
		so_long_error.c \
		map_make.c \
		map_error.c \
		put_enemy.c \
		enemy_check.c \
		enemy_move.c \
		init_texture.c \
		import_texture.c \
		import_texture2.c \
		destroy_texture.c \
		hook_key.c \
		key_up.c \
		key_down.c \
		key_left.c \
		key_right.c \
		ft_printf/ft_printf.c \
		ft_printf/ft_printf_option.c \
		ft_printf/ft_printf_option2.c \
		ft_printf/ft_printf_utils.c \
		ft_printf/ft_printf_utils2.c \
		ft_dprintf/ft_dprintf.c \
		ft_dprintf/ft_dprintf_option.c \
		ft_dprintf/ft_dprintf_option2.c \
		ft_dprintf/ft_dprintf_utils.c \
		ft_dprintf/ft_dprintf_utils2.c \
		ft_split.c

OBJS = $(SRCS:.c=.o)
INC = -I . -I minilibx-linux -I ft_printf -I ft_dprintf
MLX_FLAG = -Lminilibx-linux -lmlx -L/usr/lib -lXext -lX11 -lm -lbsd

all: $(NAME)

$(NAME): $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) $(MLX_FLAG) -o $(NAME)

bonus: $(NAME)

clean:
		rm -f $(OBJS)

fclean: clean
		rm -f $(NAME)

re: fclean all

%.o: %.c
		$(CC) $(CFLAGS) $(INC) -c $< -o $@