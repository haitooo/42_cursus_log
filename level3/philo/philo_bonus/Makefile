# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: haito <haito@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/24 00:41:46 by haito             #+#    #+#              #
#    Updated: 2025/06/27 16:16:36 by haito            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo_bonus
CC = cc
CFLAGS = -Wall -Wextra -Werror
OBJDIR = objs
SRCDIR = srcs
SRCS =	philo_main_bonus.c \
		philo_utils_bonus.c \
		philo_init_bonus.c \
		philo_error_bonus.c \
		philo_free_bonus.c \
		routines_bonus.c \
		moves_bonus.c

OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.c=.o))
INC = -I incs

all: $(NAME)

$(NAME): $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -l pthread -o $(NAME)

clean:
	$(RM) -r $(OBJDIR)

fclean: clean
		rm -f $(NAME)

re: fclean all

$(OBJDIR)/%.o:$(SRCDIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@
