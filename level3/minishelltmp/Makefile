# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: haito <haito@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/03/07 06:53:24 by haito             #+#    #+#              #
#    Updated: 2025/03/26 20:34:00 by tssaito          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
OBJDIR = objs
SRCDIR = srcs
SRCS =	main/minishell_main.c \
		main/minishell_init.c \
		main/minishell_free.c \
		main/minishell_error.c \
		main/signal.c \
		utils/minishell_utils.c \
		utils/minishell_utils2.c \
		utils/minishell_utils3.c \
		utils/minishell_utils4.c \
		utils/minishell_utils5.c \
		utils/minishell_utils6.c \
		utils/ft_split.c \
		utils/free_utils.c \
		gnl/get_next_line.c \
		gnl/get_next_line_utils.c \
		builtin/builtin.c \
		builtin/builtin_check_syntax.c \
		builtin/builtin_error.c \
		builtin/builtin_set_stdio.c \
		builtin/builtin_redirect.c \
		builtin/builtin_here_doc.c \
		builtin/builtin_here_doc_help.c \
		builtin/builtin_export.c \
		builtin/builtin_export_list.c \
		builtin/builtin_env.c \
		builtin/builtin_echo.c \
		builtin/builtin_unset.c \
		builtin/builtin_cd.c \
		builtin/builtin_exit.c \
		builtin/builtin_exit_utils.c \
		parent_process/find_brackets_pair.c \
		parent_process/sep_input_to_cmds.c \
		parent_process/handle_node.c \
		parent_process/trim_space.c \
		parent_process/pipe.c \
		parent_process/fork.c \
		parent_process/fork2.c \
		parent_process/wait.c \
		parent_process/help.c \
		parent_process/recursive.c \
		manage_varlist/manage_varlist.c \
		manage_varlist/init_varlist.c \
		expander/expander.c \
		expander/tokenizer.c \
		expander/remove_empty_tokens.c \
		expander/split_words.c \
		expander/replace_vars.c \
		expander/replace_vars_count.c \
		expander/get_words.c \
		expander/get_words_utils1.c \
		expander/get_words_utils2.c \
		expander/remove_quotes_and_split_tokens.c \
		expander/wildcard/expand_wildcard.c \
		expander/wildcard/search_same_file.c \
		expander/wildcard/sort_files.c \
		expander/wildcard/get_files.c \
		expander/wildcard/get_expanded_files.c \
		expander/wildcard/split_wildcard.c \
		expander/wildcard/wildcard_free.c \
		expander/wildcard/wildcard_utils.c \
		expander/wildcard/trim_word.c \
		expander/wildcard/manage_flag.c \
		expander/wildcard/open_wildcard.c \
		ft_eprintf/ft_eprintf.c \
		ft_eprintf/ft_eprintf_option.c \
		ft_eprintf/ft_eprintf_option2.c \
		ft_eprintf/ft_eprintf_utils.c \
		ft_eprintf/ft_eprintf_utils2.c \
		continue_child/continue_child.c \
		continue_child/exit_child.c \
		continue_child/make_cmds.c \
		continue_child/make_envp.c \
		continue_child/make_fullpath.c \
		continue_child/redirect.c \
		continue_child/here_doc.c \
		here_doc/heredoc_expand_vars.c \
		here_doc/heredoc_vars_count.c \
		here_doc/heredoc_vars_split.c 

OBJS = $(addprefix $(OBJDIR)/, $(SRCS:.c=.o))
INC = -Iincs  -Isrcs/ft_eprintf

all: $(NAME)

$(NAME): $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -l readline -o $(NAME)

bonus: $(NAME)

clean:
	$(RM) -r $(OBJDIR)

fclean: clean
		rm -f $(NAME)

re: fclean all

$(OBJDIR)/%.o:$(SRCDIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INC) -c $< -o $@
