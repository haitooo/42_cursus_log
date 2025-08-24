# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tssaito <tssaito@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/17 13:39:53 by tssaito           #+#    #+#              #
#    Updated: 2025/07/03 10:31:01 by tssaito          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

AR = ar rc

# directories
SRC_DIR = src
INC_DIR = inc
OBJ_DIR = obj

VPATH = $(SRC_DIR)

# sources
SRCS = ft_isalnum.c  ft_islower.c ft_isalpha.c  ft_isprint.c ft_isdigit.c ft_isupper.c ft_isascii.c  ft_isspace.c ft_tolower.c ft_toupper.c \
	ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
	ft_strlen.c ft_strlcat.c ft_strlcpy.c ft_strncmp.c ft_strnstr.c ft_strchr.c ft_strrchr.c \
	ft_strjoin.c ft_strmapi.c ft_strtrim.c ft_strdup.c ft_striteri.c ft_substr.c ft_split.c \
	ft_memchr.c ft_memset.c ft_memcpy.c ft_memcmp.c ft_memmove.c ft_bzero.c \
	ft_atoi.c ft_itoa.c ft_calloc.c \
	ft_lstdelone.c ft_lstnew.c ft_lstadd_back.c ft_lstiter.c ft_lstadd_front.c ft_lstlast.c ft_lstclear.c ft_lstmap.c ft_lstsize.c \
	ft_strjoin_three.c ft_strjoin_endl.c ft_strdup_endl.c \
	get_next_line.c

# objects/archives
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

# flags
WFLAG = -Wall -Wextra -Werror
IFLAG = -Iinc

# rules
all: $(NAME)

libft: $(NAME)

$(NAME): $(OBJS)
	$(AR) $(NAME) $(OBJS)

clean:
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

$(OBJ_DIR)/%.o:%.c
	mkdir -p $(@D)
	$(CC) $(WFLAG) $(IFLAG) -c $< -o $@

.PHONY: all clean fclean re
