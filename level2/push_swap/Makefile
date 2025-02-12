# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: haito <haito@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/13 02:43:32 by haito             #+#    #+#              #
#    Updated: 2025/02/13 04:06:59 by haito            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = push_swap
NAME_BONUS = checker
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCS = push_swap_main.c \
		push_swap_actions.c \
		push_swap_actions2.c \
		push_swap_actions3.c \
		push_swap_check_error.c \
		push_swap_turk.c \
		push_swap_quick.c \
		push_swap_quick_help.c \
		push_swap_turk_help.c \
		push_swap_turk_help2.c \
		push_swap_turk_help3.c \
		push_swap_turk_help4.c \
		push_swap_utils.c \
		push_swap_utils2.c \
		push_swap_utils3.c \
		ft_printf/ft_printf.c \
		ft_printf/ft_printf_option.c \
		ft_printf/ft_printf_option2.c \
		ft_printf/ft_printf_utils.c \
		ft_printf/ft_printf_utils2.c \
		ft_split.c
SRCS_BONUS = bonus/checker_main_bonus.c \
			bonus/checker_actions_bonus.c \
			bonus/checker_actions2_bonus.c \
			bonus/checker_actions3_bonus.c \
			bonus/checker_error_bonus.c \
			bonus/checker_utils_bonus.c \
			bonus/checker_utils2_bonus.c \
			ft_printf/ft_printf.c \
			ft_printf/ft_printf_option.c \
			ft_printf/ft_printf_option2.c \
			ft_printf/ft_printf_utils.c \
			ft_printf/ft_printf_utils2.c \
			ft_split.c \
			gnl/get_next_line.c \
			gnl/get_next_line_utils.c
OBJS = $(SRCS:.c=.o)
OBJS_BONUS = $(SRCS_BONUS:.c=.o)
INC = -I . -I ft_printf
INC_BONUS = -I . -I ft_printf -I gnl -I bonus

all: $(NAME)

$(NAME): $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

bonus: $(NAME_BONUS)

$(NAME_BONUS): $(OBJS_BONUS)
		$(CC) $(CFLAGS) $(OBJS_BONUS) -o $(NAME_BONUS)

clean:
		rm -f $(OBJS) $(OBJS_BONUS)

fclean: clean
		rm -f $(NAME) $(NAME_BONUS)

re: fclean all

%.o: %.c
		$(CC) $(CFLAGS) $(INC) -c $< -o $@

bonus/%.o: bonus/%.c
		$(CC) $(CFLAGS) $(INC_BONUS) -c $< -o $@