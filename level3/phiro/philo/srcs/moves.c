/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moves.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 11:48:42 by haito             #+#    #+#             */
/*   Updated: 2025/06/24 05:53:21 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	must_eat_check(t_share *share)
{
	if (share->nof_cleared >= share->nof_philo)
	{
		if (share->nof_cleared == share->nof_philo)
		{
			pthread_mutex_lock(&share->m_nof_cleared);
			share->nof_cleared++;
			pthread_mutex_unlock(&share->m_nof_cleared);
			pthread_mutex_lock(&share->m_print);
			printf("%ld all philo avoid starving\n",
				get_time_in_ms() - share->start_time + 1);
			pthread_mutex_unlock(&share->m_print);
		}
		pthread_mutex_unlock(&share->m_survival_check);
		return (CLEAR);
	}
	return (0);
}

int	survival_check(t_share *share, t_status *status)
{
	long	diff;

	pthread_mutex_lock(&share->m_survival_check);
	if (share->someone_die != SURVIVED)
	{
		pthread_mutex_unlock(&share->m_survival_check);
		return (DIE);
	}
	diff = get_time_in_ms() - status->last_meal;
	if (diff > share->time_to_die)
	{
		pthread_mutex_lock(&share->m_print);
		share->someone_die = status->id;
		printf("%ld %d died\n",
			get_time_in_ms() - share->start_time + 1, status->id);
		pthread_mutex_unlock(&share->m_print);
		pthread_mutex_unlock(&share->m_survival_check);
		return (DIE);
	}
	if (must_eat_check(share) == CLEAR)
		return (CLEAR);
	pthread_mutex_unlock(&share->m_survival_check);
	return (SURVIVED);
}

void	sleeping(t_share *share, t_status *status)
{
	pthread_mutex_lock(&share->m_print);
	if (share->someone_die == 0)
		printf("%ld %d is sleeping\n",
			get_time_in_ms() - share->start_time + 1, status->id);
	pthread_mutex_unlock(&share->m_print);
	usleep(share->time_to_sleep * 1000);
}

int	eating(t_share *share, t_status *status)
{
	status->timeof_eaten++;
	pthread_mutex_lock(&share->m_nof_cleared);
	if (status->timeof_eaten == share->nof_must_eat)
		share->nof_cleared++;
	pthread_mutex_unlock(&share->m_nof_cleared);
	status->last_meal = get_time_in_ms();
	pthread_mutex_lock(&share->m_print);
	if (share->someone_die == 0)
		printf("%ld %d is eating\n",
			get_time_in_ms() - share->start_time + 1, status->id);
	pthread_mutex_unlock(&share->m_print);
	usleep(share->time_to_eat * 1000);
	if (survival_check(share, status) == DIE)
		return (DIE);
	return (SURVIVED);
}

void	thinking(t_share *share, t_status *status, long time_to_think)
{
	pthread_mutex_lock(&share->m_print);
	if (share->someone_die == 0)
		printf("%ld %d is thinking\n",
			get_time_in_ms() - share->start_time + 1, status->id);
	pthread_mutex_unlock(&share->m_print);
	usleep(time_to_think * 1000);
}
