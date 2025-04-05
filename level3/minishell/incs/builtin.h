/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 10:40:37 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/31 07:00:24 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "includes.h"

int			check_built_in(t_status *st);
int			check_builtin_redirect_syntax(t_tokens **tokens);
int			redirect_builtin(t_tokens **tokens, t_saved *saved, char *tmpfile);
int			builtin_save_stdio(t_tokens **tokens, t_saved *saved);
int			builtin_reset_stdio(t_saved *saved);
int			builtin_error(int errnum, char *msg);
void		free_builtin_cmds(char **builtin_cmds);
t_tokens	*delete_redirect(t_tokens **tokens);
char		**init_builtin_cmds(void);
int			is_direct_builtin(t_status *st);
int			call_builtin_re(t_tokens **tokens, t_var **varlist,
				t_status *st_head, t_lp *lp);
int			call_builtin(t_tokens **tokens, t_var **varlist, t_status *st_head,
				char *tmpfile);
int			child_call_builtin(t_tokens **tokens, t_var **varlist,
				char *tmpfile);

/* builtin cmd */
int			builtin_unset(t_tokens **tokens, t_var **varlist);
int			builtin_env(t_tokens **tokens, t_var **varlist);
int			builtin_echo(t_tokens **tokens, t_var **varlist);
int			builtin_cd(t_tokens **tokens, t_var **varlist);
int			builtin_pwd(t_var **varlist);

/* export */
int			builtin_export(t_tokens **tokens, t_var **varlist, int exit_num);
int			builtin_export_list(t_var **varlist);
int			is_printable_var(char *name);

/* exit */
int			builtin_exit(t_tokens **tokens, t_var **varlist,
				t_status *st_head);
int			builtin_exit_child(t_tokens **tokens, t_var **varlist);
int			builtin_exit_re(t_tokens **tokens, t_var **varlist,
				t_status *st_head, t_lp *lp);
void		handle_exit_nonop(t_tokens **tokens);
int			get_exit_status(t_var **varlist);
int			is_numeric_argument(const char *arg);

#endif
