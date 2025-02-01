NAME = push_swap
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCS = ft_printf.c ft_printf_option.c ft_printf_option2.c ft_printf_utils.c ft_printf_utils2.c push_swap_utils.c push_swap_check_error.c push_swap_sort.c push_swap_actions.c push_swap_actions2.c main.c
OBJS = $(SRCS:.c=.o)
INC = -I .

all: $(NAME)

$(NAME): $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
		rm -f $(OBJS)

fclean: clean
		rm -f $(NAME)

re: fclean all

%.o: %.c
		$(CC) $(CFLAGS) $(INC) -c $< -o $@