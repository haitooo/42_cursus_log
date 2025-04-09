# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hito <hito@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/04 01:46:46 by haito             #+#    #+#              #
#    Updated: 2025/04/07 23:51:20 by hito             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror
OBJDIR = objs
SRCDIR = srcs
SRCS =	philo_main.c \
		philo_utils.c \
		philo_init.c \
		philo_free.c \
		philo_error.c \
		routines.c \
		printer.c

OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.c=.o))
INC = -Iincs

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
