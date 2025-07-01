/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 00:44:41 by haito             #+#    #+#             */
/*   Updated: 2025/07/01 09:00:09 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	check_invalid_args(int argc, char **argv)
{
	int	i;
	int	j;

	i = 0;
	if (argc < 5 || argc > 6)
		return (error_invalid_arg(), INVALID);
	while (++i < argc)
	{
		j = -1;
		while (argv[i][++j])
		{
			if (!my_is_digit(argv[i][j]))
				return (error_invalid_arg(), INVALID);
		}
	}
	return (SUCCESS);
}

int	create_monitor(pid_t **pids, t_status *status, t_sem *sem)
{
	(*pids)[status->nof_philo] = fork();
	if ((*pids)[status->nof_philo] < 0)
	{
		write(2, "philo: fork failed\n", 19);
		return (ERROR);
	}
	else if ((*pids)[status->nof_philo] == 0)
		monitor(status, sem, pids);
	return (SUCCESS);
}

int	create_processes(pid_t **pids, t_status *status, t_sem *sem)
{
	int	i;

	*pids = malloc(sizeof(pid_t) * (status->nof_philo + 1));
	if (!*pids)
		return (error_malloc(), ERROR);
	i = -1;
	status->start_time = get_time_in_ms();
	if (create_monitor(pids, status, sem) == ERROR)
		return (ERROR);
	while (++i < status->nof_philo)
	{
		status->id = i + 1;
		(*pids)[i] = fork();
		if ((*pids)[i] < 0)
		{
			write(2, "philo: fork failed\n", 19);
			while (--i >= 0)
				kill((*pids)[i], SIGKILL);
			return (ERROR);
		}
		else if ((*pids)[i] == 0)
			routine(status, sem, pids);
	}
	return (SUCCESS);
}

void	wait_prosess(pid_t *pids, t_status *status)
{
	int		j;
	int		return_code;
	int		code;
	pid_t	pid;

	pid = waitpid(-1, &return_code, 0);
	j = 0;
	while (j < status->nof_philo)
	{
		kill(pids[j], SIGKILL);
		j++;
	}
	if (pid > 0 && WIFEXITED(return_code))
	{
		code = WEXITSTATUS(return_code);
		if (code == status->nof_philo + 1)
			printf("%ld all philo avoid starving\n",
				get_time_in_ms() - status->start_time + 1);
		else
			printf("%ld %d died\n",
				get_time_in_ms() - status->start_time + 1, code);
	}
}

int	main(int argc, char **argv)
{
	t_status	*status;
	t_sem		*sem;
	pid_t		*pids;

	unlink_sem();
	if (check_invalid_args(argc, argv) == INVALID)
		return (FAILED);
	if (init_structs(&status, --argc, ++argv) == ERROR)
		return (FAILED);
	if (init_sem(&sem, status) == ERROR)
		return (free(status), FAILED);
	if (create_processes(&pids, status, sem) == ERROR)
		return (free_sem(&sem, 0), free(status), FAILED);
	wait_prosess(pids, status);
	free_sem(&sem, 0);
	free(status);
	free(pids);
	return (0);
}
