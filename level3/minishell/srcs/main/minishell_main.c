/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 18:40:23 by haito             #+#    #+#             */
/*   Updated: 2025/04/04 00:12:14 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

int	continue_line(char *input, t_var **varlist)
{
	t_status	*state;
	t_brackets	brackets;

	if (signal(SIGINT, sig_handler_inprocess) == SIG_ERR)
		return (perror("minishell: signal"), update_exit_code(1, varlist),
			ERROR);
	if (signal(SIGQUIT, sig_handler_inprocess) == SIG_ERR)
		return (perror("minishell: signal"), update_exit_code(1, varlist),
			ERROR);
	if (!input)
		return (0);
	if (find_brackets_pair(input, &brackets, ft_strlen(input),
			varlist) == ERROR)
		return (ERROR);
	state = sep_input_to_cmds(input, &brackets, NULL, varlist);
	if (!state)
		return (ERROR);
	if (make_pipe(&state, varlist) == ERROR)
		return (free_lst_status(state, NULL), ERROR);
	if (check_heredoc(&state, varlist) == ERROR)
		return (free_lst_status(state, NULL), ERROR);
	if (fork_and_wait(&state, varlist) == ERROR)
		return (free_lst_status(state, NULL), ERROR);
	delete_tmpfile(state);
	return (free_lst_status(state, NULL), SUCCESS);
}

void	main_loop(t_var **varlist)
{
	char	*input;
	int		result;

	while (1)
	{
		setup_signal_handlers(varlist);
		input = readline("minishell$ ");
		if (g_signal == SIGINT)
			update_exit_code(130, varlist);
		g_signal = 0;
		if (!input)
		{
			printf("exit\n");
			result = get_exit_status(varlist);
			free_varlist(varlist);
			exit(result);
		}
		if (*input)
			add_history(input);
		continue_line(input, varlist);
		free(input);
		input = NULL;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_var	*varlist;
	char	*name_dup;
	char	*value_dup;

	if (argc != 1)
		return (ft_eprintf("minishell: too many argument\n"), FAILED);
	(void)argv;
	name_dup = ft_strdup("?");
	if (!name_dup)
		return (error_node(ERRNO_ONE));
	value_dup = ft_strdup("0");
	if (!value_dup)
		return (free(name_dup), error_node(ERRNO_ONE));
	varlist = init_varlist(envp, name_dup, value_dup);
	count_up_shlvl(&varlist);
	main_loop(&varlist);
	return (free_varlist(&varlist), 0);
}
