/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 11:03:24 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/31 06:56:31 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDES_H
# define INCLUDES_H

# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <signal.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <readline/history.h>
# include <readline/readline.h>
# include "struct.h"
# include "error_num.h"
# include "minishell.h"
# include "manage_varlist.h"
# include "expander.h"
# include "continue_child.h"
# include "builtin.h"
# include "utils.h"
# include "ft_eprintf.h"
# include "heredoc.h"
# include "my_signal.h"

extern volatile sig_atomic_t	g_signal;

#endif
