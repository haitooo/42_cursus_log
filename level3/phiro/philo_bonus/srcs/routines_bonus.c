/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 05:21:45 by haito             #+#    #+#             */
/*   Updated: 2025/06/24 13:48:42 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	monitor(t_status *status, t_sem *sem, pid_t **pids)
{
	int	i;
	int	id;

	i = 0;
	id = status->nof_philo + 1;
	while (i < status->nof_philo)
	{
		sem_wait(sem->sem_cleared);
		i++;
	}
	free_sem(&sem, 0);
	free(status);
	free(*pids);
	exit(id);
}

void	routine_(t_status *status, t_sem *sem, pid_t **pids)
{
	sem_wait(sem->sem_fork);
	survival_check(status, sem, pids);
	sem_wait(sem->sem_print);
	printf("%ld %d has taken a fork\n",
		get_time_in_ms() - status->start_time + 1, status->id);
	sem_post(sem->sem_print);
	sem_wait(sem->sem_fork);
	survival_check(status, sem, pids);
	sem_wait(sem->sem_print);
	printf("%ld %d has taken a fork\n",
		get_time_in_ms() - status->start_time + 1, status->id);
	sem_post(sem->sem_print);
	eating(status, sem, pids);
	sem_post(sem->sem_fork);
	sem_post(sem->sem_fork);
	sleeping(status, sem, pids);
}

int	case_oddphilos(t_status *status, t_sem *sem, int is_first, pid_t **pids)
{
	while (1)
	{
		if (!is_first)
			survival_check(status, sem, pids);
		if (status->id <= status->nof_philo / 2)
			routine_(status, sem, pids);
		else if (status->id == status->nof_philo)
		{
			if (is_first)
				thinking(status, sem,
					(status->time_to_eat) + (status->time_to_eat / 2), pids);
			else
				thinking(status, sem,
					(status->time_to_eat / 2), pids);
			routine_(status, sem, pids);
		}
		else
		{
			if (is_first)
				thinking(status, sem, ((status->time_to_eat / 3) * 2), pids);
			routine_(status, sem, pids);
		}
		is_first = 0;
	}
	return (0);
}

int	case_evenphilos(t_status *status, t_sem *sem, int is_first, pid_t **pids)
{
	while (1)
	{
		if (!is_first)
			survival_check(status, sem, pids);
		if (status->id <= status->nof_philo / 2)
		{
			routine_(status, sem, pids);
		}
		else
		{
			if (is_first)
			{
				thinking(status, sem,
					status->time_to_eat, pids);
			}
			routine_(status, sem, pids);
		}
		if (status->time_to_eat > (status->time_to_sleep + 10))
			thinking(status, sem,
				status->time_to_eat - (status->time_to_sleep + 10), pids);
		is_first = 0;
	}
	return (0);
}

void	routine(t_status *status, t_sem *sem, pid_t **pids)
{
	status->last_meal = get_time_in_ms();
	if (status->nof_philo % 2 != 0)
		case_oddphilos(status, sem, 1, pids);
	else
		case_evenphilos(status, sem, 1, pids);
	free_sem(&sem, 0);
	free(status);
	free(*pids);
	exit(0);
}
