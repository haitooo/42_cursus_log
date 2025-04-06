/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 01:34:03 by hito              #+#    #+#             */
/*   Updated: 2025/04/06 21:16:22 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	sleeping(t_share *share, t_status *status)
{
	pthread_mutex_lock(&share->m_print);
	printf("%d:sleeping\n", status->id);
	pthread_mutex_unlock(&share->m_print);
	usleep(share->time_to_sleep);
}

void	eating(t_share *share, t_status *status)
{
	pthread_mutex_lock(&share->m_print);
	printf("%d:eating\n", status->id);
	pthread_mutex_unlock(&share->m_print);
	usleep(share->time_to_eat);
}

void	thinking(t_share *share, t_status *status)
{
	pthread_mutex_lock(&share->m_print);
	printf("%d:thinking\n", status->id);
	pthread_mutex_unlock(&share->m_print);
	usleep(share->time_to_eat);
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
	while (share->start == 0)
		;
	while (1)
	{
		if (status->id % 2 != 0)
		{
			if (pthread_mutex_lock(status->my_fork_l) == 0)
			{
				if (pthread_mutex_lock(status->my_fork_r) == 0)
				{
					eating(share, status);
					pthread_mutex_unlock(status->my_fork_l);
					pthread_mutex_unlock(status->my_fork_r);
					sleeping(share, status);
					thinking(share, status);
				}
			}
		}
		else
		{
			if (pthread_mutex_lock(status->my_fork_r) == 0)
			{
				if (pthread_mutex_lock(status->my_fork_l) == 0)
				{
					eating(share, status);
					pthread_mutex_unlock(status->my_fork_l);
					pthread_mutex_unlock(status->my_fork_r);
					sleeping(share, status);
					thinking(share, status);
				}
			}
		}
	}
	return (NULL);
}
