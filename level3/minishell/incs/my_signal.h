/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_signal.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 06:50:44 by haito             #+#    #+#             */
/*   Updated: 2025/03/31 07:07:28 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MY_SIGNAL_H
# define MY_SIGNAL_H

# include "includes.h"

void		sig_handler_inprocess(int signal);
void		sig_handler_heredoc(int signal);
void		sigint_handler(int signal);
void		setup_signal_handlers(t_var **varlist);
int			check_signal(t_lp *lp);
void		exit_child_sigint(t_child *child, char *buf, char *file);

#endif