/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 22:11:26 by haito             #+#    #+#             */
/*   Updated: 2025/03/27 03:14:32 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "includes.h"

char		**init_builtin_cmds(void);
void		free_builtin_cmds(char **builtin_cmds);

int			find_brackets_pair(const char *input, t_brackets *b, int length,
				t_var **var);
int			get_brackets_pair(int i, t_brackets *brackets);
int			add_operator_node(const char *op, char **cmds, t_status **st_head,
				t_parser *ps);
t_status	*ft_new_node(const char *cmds, int has_brackets);
void		ft_add_back_node(t_status **head, t_status *new_node);
void		ft_remove_node(t_status **head, t_status *node);
int			continue_line(char *input, t_var **varlist);

// changed
char		*trim_spaces(char *str);
char		*add_char(char *cmds, char c, int *i, t_var **var);
int			add_command_node(char **cmds, t_status **st_head, t_var **var);
t_status	*sep_input_to_cmds(const char *input, t_brackets *brackets,
				t_status *st_head, t_var **var);
int			make_pipe(t_status **st_head, t_var **var);
int			fork_and_wait(t_status **st_head, t_var **varlist);

// new
void		*error_add_char(int *i, char *cmds);
void		free_lst_status(t_status *st_head, t_status *st);
int			error_handle_brackets(int error_num, t_var **var);
int			error_node(int error_num);
int			error_pipe(int error_num, t_var **var, int ope, char *cmds);
int			call_builtin(t_tokens **tokens, t_var **varlist, t_status *st_head);
void		handle_and_or(t_status *st, t_lp *lp, t_var **var);
void		handle_parent_process(t_status *st);
void		handle_child_process(t_status *st, t_var **varlist,
				t_status *st_head);
void		frees(t_status *st_head, t_var **varlist);
int			recursive_continue_line(char *input, t_var **varlist);
int			child_call_builtin(t_tokens **tokens, t_var **varlist);
int			fork_and_wait_(t_status **st_head, t_var **varlist, char *input);

void		sigint_handler(int signal);
void		sig_handler_inprocess(int signal);
void		sig_ignore(int signal);
void		sig_handler_heredoc(int signal);
int			check_built_in(t_status *st);
int			expand_cmds(t_status **st_head, t_var **varlist);
int			count_up_shlvl(t_var **varlist);
void		exit_child_sigint(t_child *child, char *buf, char *file);
void		heredoc_loop_builtin(int fd, char *limiter, t_type type,
				t_var **varlist);
int			update_exit_code(int exit_code, t_var **varlist);
int			wait_process(t_lp *lp, t_var **varlist, t_status **st_head);
int			ft_atoi_exit(const char *str);
int			get_exit_status(t_var **varlist);
int			call_builtin_re(t_tokens **tokens, t_var **varlist,
				t_status *st_head, char *in);
int			is_direct_builtin(t_status *st);
char		*get_path(t_var **varlist, char *path);
char		*trim_edges_space(const char *str);
int			has_heredoc(t_status *st);

#endif
