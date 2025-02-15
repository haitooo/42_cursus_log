# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: haito <haito@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/13 16:31:09 by haito             #+#    #+#              #
#    Updated: 2025/02/13 16:31:10 by haito            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCS =	pipex_main.c \
		pipex_utils.c \
		pipex_init.c \
		pipex_get_path.c \
		pipex_free.c \
		pipex_error.c \
		pipex_error2.c \
		pipex_child_process.c \
		ft_printf/ft_printf.c \
		ft_printf/ft_printf_option.c \
		ft_printf/ft_printf_option2.c \
		ft_printf/ft_printf_utils.c \
		ft_printf/ft_printf_utils2.c \
		ft_fprintf/ft_fprintf.c \
		ft_fprintf/ft_fprintf_option.c \
		ft_fprintf/ft_fprintf_option2.c \
		ft_fprintf/ft_fprintf_utils.c \
		ft_fprintf/ft_fprintf_utils2.c \
		gnl/get_next_line.c \
		gnl/get_next_line_utils.c \
		ft_split.c
OBJS = $(SRCS:.c=.o)
INC = -I . -I ft_printf -I ft_fprintf -I gnl

all: $(NAME)

$(NAME): $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

bonus: $(NAME)

clean:
		rm -f $(OBJS)

fclean: clean
		rm -f $(NAME)

re: fclean all

%.o: %.c
		$(CC) $(CFLAGS) $(INC) -c $< -o $@
