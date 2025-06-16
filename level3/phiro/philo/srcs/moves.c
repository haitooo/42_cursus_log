/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moves.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 11:48:42 by haito             #+#    #+#             */
/*   Updated: 2025/04/11 11:49:13 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
