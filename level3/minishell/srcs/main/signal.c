/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 03:16:18 by haito             #+#    #+#             */
/*   Updated: 2025/03/26 15:48:41 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_handler_inprocess(int signal)
{
	if (signal == SIGINT)
		g_signal = SIGINT;
	if (signal == SIGQUIT)
		g_signal = SIGQUIT;
}

void	sig_handler_heredoc(int signal)
{
	g_signal = signal;
	close(STDIN_FILENO);
}

void	sigint_handler(int signal)
{
	(void)signal;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}
