# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: haito <haito@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/23 19:28:46 by haito             #+#    #+#              #
#    Updated: 2024/11/25 04:01:26 by haito            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libftprintf.a
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCS = ft_printf.c ft_printf_option.c ft_printf_option2.c ft_printf_option3.c ft_printf_utils.c ft_printf_utils2.c ft_printf_flag.c ft_printf_flag2.c
OBJS = $(SRCS:.c=.o)
INC = -I ft_printf.h

all: $(NAME)

bonus: all

$(NAME): $(OBJS)
		ar cr $(NAME) $(OBJS) libft.a

clean:
		rm -f $(OBJS)

fclean: clean
		rm -f $(NAME)

re: fclean all

%.o: %.c
		$(CC) $(CFLAGS) $(INC) -c $< -o $@