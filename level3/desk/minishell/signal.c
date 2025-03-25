/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 03:16:18 by haito             #+#    #+#             */
/*   Updated: 2025/03/22 08:35:49 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_ignore(int signal)
{
	(void)signal;
}

void	sigint_handler_heredoc(int signal)
{
	(void)signal;
	g_signal = SIGINT;
	close(STDIN_FILENO);
}

void	sigint_handler_inprocess(int signal)
{
	(void)signal;
	g_signal = SIGINT;
}

void	sigint_handler(int signal)
{
	(void)signal;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}
