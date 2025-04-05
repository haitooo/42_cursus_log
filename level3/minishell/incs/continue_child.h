/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   continue_child.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 10:49:29 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/31 01:49:28 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONTINUE_CHILD_H
# define CONTINUE_CHILD_H

# include "includes.h"

/* continue_child */
void	continue_child(t_tokens **tokens, t_var **varlist, char *tmpfile);
void	redirect_fds(t_child *child, t_tokens **tokens);
void	make_cmds(t_child *child, t_tokens **tokens);
void	make_envp(t_child *child, t_var **varlist);
void	make_fullpath(t_child *child, char *cmd, t_var **varlist);
void	exit_child(t_child *child, int status, int errnum, char *msg);

#endif
