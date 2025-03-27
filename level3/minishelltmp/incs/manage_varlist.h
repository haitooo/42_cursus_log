/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_varlist.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 10:50:18 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/24 11:05:00 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MANAGE_VARLIST_H
# define MANAGE_VARLIST_H

# include "includes.h"

t_var	*init_varlist(char **envp, char *c1, char *c2);
void	free_varlist(t_var **varlist);
t_var	*get_var(t_var **varlist, char *name);
void	add_var(t_var **varlist, char *var_name, char *var_value);

#endif
