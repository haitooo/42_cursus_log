/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hito <hito@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 01:34:03 by hito              #+#    #+#             */
/*   Updated: 2025/04/08 23:12:35 by hito             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// int	survival_check(t_share *share, t_status *status)
// {
// 	long	diff;

// 	pthread_mutex_lock(&share->m_survival_check);
// 	if (share->someone_die == 1 || get_time_in_ms() - status->last_meal > share->time_to_die)
// 	{
// 		share->someone_die = 1;
// 		pthread_mutex_unlock(&share->m_survival_check);
// 		return (DIE);
// 	}
// 	pthread_mutex_unlock(&share->m_survival_check);
// 	return (SURVIVED);
// }

int	survival_check(t_share *share, t_status *status)
{
	long	diff;

	pthread_mutex_lock(&share->m_survival_check);
	if (share->someone_die != SURVIVED)
	{
		pthread_mutex_unlock(&share->m_survival_check);
		return (DIE);
	}
	pthread_mutex_lock(&status->m_last_meal);
	diff = get_time_in_ms() - status->last_meal;
	pthread_mutex_unlock(&status->m_last_meal);
	if (diff > share->time_to_die)
	{
		pthread_mutex_lock(&share->m_print);
		share->someone_die = status->id;
		printf("%ld %d died\n", get_time_in_ms() - share->start_time + 1, status->id);
		pthread_mutex_unlock(&share->m_print);
		pthread_mutex_unlock(&share->m_survival_check);
		return (DIE);
	}
	pthread_mutex_unlock(&share->m_survival_check);
	return (SURVIVED);
}


void	sleeping(t_share *share, t_status *status)
{
	pthread_mutex_lock(&share->m_print);
	if (share->someone_die == 0)
		printf("%ld %d is sleeping\n", get_time_in_ms() - share->start_time + 1, status->id);
	pthread_mutex_unlock(&share->m_print);
	usleep(share->time_to_sleep * 1000);
}

int	eating(t_share *share, t_status *status)
{
	pthread_mutex_lock(&status->m_last_meal);
	status->last_meal = get_time_in_ms();
	pthread_mutex_lock(&share->m_print);
	if (share->someone_die == 0)
		printf("%ld %d is eating\n", get_time_in_ms() - share->start_time + 1, status->id);
	pthread_mutex_unlock(&share->m_print);
	pthread_mutex_unlock(&status->m_last_meal);
	usleep(share->time_to_eat * 1000);
	if (survival_check(share, status) == DIE)
		return (DIE);
	return (SURVIVED);
}

void	thinking(t_share *share, t_status *status, long time_to_think)
{
	pthread_mutex_lock(&share->m_print);
	if (share->someone_die == 0)
		printf("%ld %d is thinking\n", get_time_in_ms() - share->start_time + 1, status->id);
	pthread_mutex_unlock(&share->m_print);
	usleep(time_to_think * 1000);
}

void	*routine(void *arg)
{
	t_status	*status;
	t_share		*share;

	status = (t_status *)arg;
	share = status->share;
	status->my_fork_l = &share->m_fork[status->id - 1];
	if (status->id == share->nof_philo)
		status->my_fork_r = &share->m_fork[0];
	else
		status->my_fork_r = &share->m_fork[status->id];
	while (share->start_flag != START && share->create_error == 0)
		usleep(1);
	if (share->create_error == ERROR)
		return (NULL);
	status->last_meal = get_time_in_ms();
	while (1)
	{
		if (survival_check(share, status) == DIE)
			return (NULL);
		if (status->id % 2 != 0)
		{
			if (pthread_mutex_lock(status->my_fork_l) == 0)
			{
				if (survival_check(share, status) == DIE)
				{
					pthread_mutex_unlock(status->my_fork_l);
					return (NULL);
				}
				pthread_mutex_lock(&share->m_print);
				if (share->someone_die == 0)
					printf("%ld %d has taken a fork\n", get_time_in_ms() - share->start_time + 1, status->id);
				pthread_mutex_unlock(&share->m_print);
				if (pthread_mutex_lock(status->my_fork_r) == 0)
				{
					if (survival_check(share, status) == DIE)
					{
						pthread_mutex_unlock(status->my_fork_l);
						pthread_mutex_unlock(status->my_fork_r);
						return (NULL);
					}
					pthread_mutex_lock(&share->m_print);
					if (share->someone_die == 0)
						printf("%ld %d has taken a fork\n", get_time_in_ms() - share->start_time + 1, status->id);
					pthread_mutex_unlock(&share->m_print);
					if (eating(share, status) == DIE)
					{
						pthread_mutex_unlock(status->my_fork_l);
						pthread_mutex_unlock(status->my_fork_r);
						return (NULL);
					}
					pthread_mutex_unlock(status->my_fork_l);
					pthread_mutex_unlock(status->my_fork_r);
					sleeping(share, status);
					// if (share->nof_philo % 2 != 0 && status->id == share->nof_philo - 2)
					// 	thinking(share, status, 1);
					if (share->time_to_eat > (share->time_to_sleep + 1000))
						thinking(share, status, share->time_to_eat - (share->time_to_sleep + 1000) + 1);
				}
			}
		}
		else
		{
			usleep(500);
			if (pthread_mutex_lock(status->my_fork_r) == 0)
			{
				if (survival_check(share, status) == DIE)
				{
					pthread_mutex_unlock(status->my_fork_r);
					return (NULL);
				}
				pthread_mutex_lock(&share->m_print);
				if (share->someone_die == 0)
					printf("%ld %d has taken a fork\n", get_time_in_ms() - share->start_time + 1, status->id);
				pthread_mutex_unlock(&share->m_print);
				if (pthread_mutex_lock(status->my_fork_l) == 0)
				{
					if (survival_check(share, status) == DIE)
					{
						pthread_mutex_unlock(status->my_fork_r);
						pthread_mutex_unlock(status->my_fork_l);
						return (NULL);
					}
					pthread_mutex_lock(&share->m_print);
					if (share->someone_die == 0)
						printf("%ld %d has taken a fork\n", get_time_in_ms() - share->start_time + 1, status->id);
					pthread_mutex_unlock(&share->m_print);
					if (eating(share, status) == DIE)
					{
						pthread_mutex_unlock(status->my_fork_l);
						pthread_mutex_unlock(status->my_fork_r);
						return (NULL);
					}
					pthread_mutex_unlock(status->my_fork_r);
					pthread_mutex_unlock(status->my_fork_l);
					sleeping(share, status);
					if (share->time_to_eat > (share->time_to_sleep + 1000))
						thinking(share, status, share->time_to_eat - (share->time_to_sleep + 1000));
				}
			}
		}
	}
	return (NULL);
}
