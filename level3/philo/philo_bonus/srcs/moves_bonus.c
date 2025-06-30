/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moves_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 05:36:30 by haito             #+#    #+#             */
/*   Updated: 2025/06/30 21:17:25 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	survival_check(t_status *status, t_sem *sem, pid_t **pids)
{
	long	diff;
	int		id;

	sem_wait(sem->sem_survival_check);
	diff = get_time_in_ms() - status->last_meal;
	if (diff > status->time_to_die)
	{
		sem_wait(sem->sem_print);
		id = status->id;
		free_sem(&sem, 0);
		free(status);
		free(*pids);
		exit (id);
	}
	sem_post(sem->sem_survival_check);
}

void	sleeping(t_status *status, t_sem *sem, pid_t **pids)
{
	long	time;

	sem_wait(sem->sem_print);
	printf("%ld %d is sleeping\n",
		get_time_in_ms() - status->start_time + 1, status->id);
	sem_post(sem->sem_print);
	time = status->time_to_sleep * 1000;
	while (time > 0)
	{
		if (time < SLEEP)
		{
			usleep(time);
			return ;
		}
		usleep(SLEEP);
		time -= SLEEP;
		survival_check(status, sem, pids);
	}
}

void	eating(t_status *status, t_sem *sem, pid_t **pids)
{
	long	time;

	status->last_meal = get_time_in_ms();
	sem_wait(sem->sem_print);
	printf("%ld %d is eating\n",
		get_time_in_ms() - status->start_time + 1, status->id);
	sem_post(sem->sem_print);
	time = status->time_to_eat * 1000;
	while (time > 0)
	{
		if (time < SLEEP)
		{
			usleep(time);
			return ;
		}
		usleep(SLEEP);
		time -= SLEEP;
		survival_check(status, sem, pids);
	}
	status->timeof_eaten++;
	if (status->timeof_eaten == status->nof_must_eat)
		sem_post(sem->sem_cleared);
}

void	thinking(t_status *status, t_sem *sem, long time_to_think, pid_t **pids)
{
	long	time;

	sem_wait(sem->sem_print);
	printf("%ld %d is thinking\n",
		get_time_in_ms() - status->start_time + 1, status->id);
	sem_post(sem->sem_print);
	time = time_to_think * 1000;
	while (time > 0)
	{
		if (time < SLEEP)
		{
			usleep(time);
			return ;
		}
		usleep(SLEEP);
		time -= SLEEP;
		survival_check(status, sem, pids);
	}
}
