NAME = push_swap
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRCS = main.c \
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
		push_swap_utils.c \
		ft_printf/ft_printf.c \
		ft_printf/ft_printf_option.c \
		ft_printf/ft_printf_option2.c \
		ft_printf/ft_printf_utils.c \
		ft_printf/ft_printf_utils2.c
OBJS = $(SRCS:.c=.o)
INC = -I . -I ft_printf

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