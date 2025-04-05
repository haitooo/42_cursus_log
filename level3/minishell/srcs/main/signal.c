/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 03:16:18 by haito             #+#    #+#             */
/*   Updated: 2025/04/04 00:13:03 by haito            ###   ########.fr       */
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
	write(STDOUT_FILENO, "\n", 1);
	close(STDIN_FILENO);
}

void	sigint_handler(int signal)
{
	(void)signal;
	g_signal = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	setup_signal_handlers(t_var **varlist)
{
	g_signal = 0;
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
	{
		perror("minishell: signal");
		free_varlist(varlist);
		exit(1);
	}
	if (signal(SIGINT, sigint_handler) == SIG_ERR)
	{
		perror("minishell: signal");
		free_varlist(varlist);
		exit(1);
	}
}
