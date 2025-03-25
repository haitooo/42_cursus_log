/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 10:40:37 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/24 11:50:15 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "includes.h"

int		check_builtin_redirect_syntax(t_tokens **tokens);
int		redirect_builtin(t_tokens **tokens, t_saved *saved, t_var **varlist);
int		check_here_doc(t_tokens **tokens, char **tmpfile, t_var **varlist);
int		builtin_save_stdio(t_tokens **tokens, t_saved *saved);
int		builtin_reset_stdio(t_saved *saved);
int		builtin_error(int errnum, char *msg);

/* builtin cmd */
int		builtin_unset(t_tokens **tokens, t_var **varlist);
int		builtin_env(t_tokens **tokens, t_var **varlist);
int		builtin_echo(t_tokens **tokens, t_var **varlist);
int		builtin_cd(t_tokens **tokens, t_var **varlist);
int		builtin_pwd(void);

/* export */
int		builtin_export(t_tokens **tokens, t_var **varlist, int exit_num);
int		builtin_export_list(t_var **varlist);

/* exit */
int		builtin_exit(t_tokens **tokens, t_var **varlist, t_status *st_head);
int		builtin_exit_child(t_tokens **tokens, t_var **varlist);
int		builtin_exit_re(t_tokens **tokens, t_var **varlist, t_status *st_head,
			char *in);
void	handle_exit_nonop(t_tokens **tokens);
int		get_exit_status(t_var **varlist);
int		is_numeric_argument(const char *arg);

#endif
