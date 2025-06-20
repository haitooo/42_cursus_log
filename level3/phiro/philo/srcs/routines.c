/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 01:34:03 by hito              #+#    #+#             */
/*   Updated: 2025/06/20 20:52:42 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	routine_even(t_share *share, t_status *status)
{
	pthread_mutex_lock(status->my_fork_r);
	if (survival_check(share, status) == DIE)
		return (pthread_mutex_unlock(status->my_fork_r), ERROR);
	pthread_mutex_lock(&share->m_print);
	if (share->someone_die == 0)
		printf("%ld %d has taken a fork\n",
			get_time_in_ms() - share->start_time + 1, status->id);
	pthread_mutex_unlock(&share->m_print);
	pthread_mutex_lock(status->my_fork_l);
	if (survival_check(share, status) == DIE)
		return (pthread_mutex_unlock(status->my_fork_l),
			pthread_mutex_unlock(status->my_fork_r), ERROR);
	pthread_mutex_lock(&share->m_print);
	if (share->someone_die == 0)
		printf("%ld %d has taken a fork\n",
			get_time_in_ms() - share->start_time + 1, status->id);
	pthread_mutex_unlock(&share->m_print);
	if (eating(share, status) == DIE)
		return (pthread_mutex_unlock(status->my_fork_l),
			pthread_mutex_unlock(status->my_fork_r), ERROR);
	pthread_mutex_unlock(status->my_fork_l);
	pthread_mutex_unlock(status->my_fork_r);
	sleeping(share, status);
	return (0);
}

int	routine_(t_share *share, t_status *status)
{
	pthread_mutex_lock(status->my_fork_l);
	if (survival_check(share, status) == DIE)
		return (pthread_mutex_unlock(status->my_fork_l), ERROR);
	pthread_mutex_lock(&share->m_print);
	if (share->someone_die == 0)
		printf("%ld %d has taken a fork\n",
			get_time_in_ms() - share->start_time + 1, status->id);
	pthread_mutex_unlock(&share->m_print);
	pthread_mutex_lock(status->my_fork_r);
	if (survival_check(share, status) == DIE)
		return (pthread_mutex_unlock(status->my_fork_r),
			pthread_mutex_unlock(status->my_fork_l), ERROR);
	pthread_mutex_lock(&share->m_print);
	if (share->someone_die == 0)
		printf("%ld %d has taken a fork\n",
			get_time_in_ms() - share->start_time + 1, status->id);
	pthread_mutex_unlock(&share->m_print);
	if (eating(share, status) == DIE)
		return (pthread_mutex_unlock(status->my_fork_r),
			pthread_mutex_unlock(status->my_fork_l), ERROR);
	pthread_mutex_unlock(status->my_fork_r);
	pthread_mutex_unlock(status->my_fork_l);
	sleeping(share, status);
	return (0);
}

int	case_oddphilos(t_share *share, t_status *status, int is_first, long time)
{
	while (1)
	{
		if (!is_first && survival_check(share, status) == DIE)
			return (ERROR);
		if (status->id % 3 == 2)
			thinking(share, status, (share->time_to_eat / 2));
		else if (status->id % 3 == 1 || status->id % 3 == 2)
		{
			if (routine_(share, status) == ERROR)
				return (ERROR);
			thinking(share, status, time);
		}
		else if (status->id % 3 == 0)
		{
			if (is_first)
				thinking(share, status,
					(share->time_to_eat) + (share->time_to_eat / 2));
			else
				thinking(share, status, time);
			if (routine_(share, status) == ERROR)
				return (ERROR);
		}
		is_first = 0;
	}
	return (0);
}

int	case_evenphilos(t_share *share, t_status *status, int is_first)
{
	while (1)
	{
		if (!is_first && survival_check(share, status) == DIE)
			return (ERROR);
		if (status->id % 2 != 0)
		{
			if (routine_(share, status) == ERROR)
				return (ERROR);
		}
		else
		{
			if (is_first)
				usleep(50);
			if (routine_even(share, status) == ERROR)
				return (ERROR);
		}
		if (share->time_to_eat > (share->time_to_sleep + 10))
			thinking(share, status,
				share->time_to_eat - (share->time_to_sleep + 10));
		is_first = 0;
	}
	return (0);
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
	if (share->nof_philo % 2 != 0)
		case_oddphilos(share, status, 1,
			((share->time_to_eat / 2) * 3) - share->time_to_sleep);
	else
		case_evenphilos(share, status, 1);
	return (NULL);
}
