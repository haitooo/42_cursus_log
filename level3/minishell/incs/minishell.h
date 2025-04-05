/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 22:11:26 by haito             #+#    #+#             */
/*   Updated: 2025/03/31 07:13:08 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "includes.h"

int			count_up_shlvl(t_var **varlist);
int			find_brackets_pair(const char *input, t_brackets *b, int length,
				t_var **var);
int			get_brackets_pair(int i, t_brackets *brackets);
t_status	*sep_input_to_cmds(const char *input, t_brackets *brackets,
				t_status *st_head, t_var **var);
int			make_pipe(t_status **st_head, t_var **var);
int			fork_and_wait(t_status **st_head, t_var **varlist);
void		fork_process(t_status *st, t_var **varlist,
				t_lp *lp, t_status *st_head);
int			wait_process(t_lp *lp, t_var **varlist, t_status **st_head);
int			prepare_and_expand_tokens(t_status *st, t_var **varlist);
int			recursive_continue_line(char *input, t_var **varlist,
				char *heredoc);
int			fork_and_wait_(t_status **st_head, t_var **varlist, char *input,
				char *heredoc);
void		handle_and_or(t_status *st, t_lp *lp, t_var **var);
void		handle_parent_process(t_status *st);
void		handle_child_process(t_status *st, t_var **varlist,
				t_status *st_head, char *heredoc);
void		handle_parent_process(t_status *st);
void		handle_and_or(t_status *st, t_lp *lp, t_var **var);
void		handle_main_wait(t_status *st, t_lp *lp);
void		handle_pipe_wait(t_status *st, t_lp *lp);
int			handle_brackets(const char *input, t_brackets *brackets,
				t_status **st_head, t_parser *ps);
int			handle_quotes(const char *input, char **cmds, int i, t_var **var);
int			skip_spaces(const char *input, int i);

#endif
