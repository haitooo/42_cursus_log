/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 01:34:03 by hito              #+#    #+#             */
/*   Updated: 2025/06/27 12:48:23 by haito            ###   ########.fr       */
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

int	routine_odd(t_share *share, t_status *status)
{
	pthread_mutex_lock(status->first);
	if (survival_check(share, status) == DIE)
		return (pthread_mutex_unlock(status->first), ERROR);
	pthread_mutex_lock(&share->m_print);
	if (share->someone_die == 0)
		printf("%ld %d has taken a fork\n",
			get_time_in_ms() - share->start_time + 1, status->id);
	pthread_mutex_unlock(&share->m_print);
	pthread_mutex_lock(status->second);
	if (survival_check(share, status) == DIE)
		return (pthread_mutex_unlock(status->second),
			pthread_mutex_unlock(status->first), ERROR);
	pthread_mutex_lock(&share->m_print);
	if (share->someone_die == 0)
		printf("%ld %d has taken a fork\n",
			get_time_in_ms() - share->start_time + 1, status->id);
	pthread_mutex_unlock(&share->m_print);
	if (eating(share, status) == DIE)
		return (pthread_mutex_unlock(status->second),
			pthread_mutex_unlock(status->first), ERROR);
	pthread_mutex_unlock(status->second);
	pthread_mutex_unlock(status->first);
	sleeping(share, status);
	return (0);
}

void	*routine(void *arg)
{
	t_status	*status;
	t_share		*share;
	int			local_flag;

	status = (t_status *)arg;
	share = status->share;
	init_forks(share, status);
	pthread_mutex_lock(&share->m_start);
	local_flag = share->start_flag;
	pthread_mutex_unlock(&share->m_start);
	while (local_flag != START && share->create_error == 0)
		usleep(1);
	if (share->create_error == ERROR)
		return (NULL);
	pthread_mutex_lock(&status->m_last_meal);
	status->last_meal = get_time_in_ms();
	pthread_mutex_unlock(&status->m_last_meal);
	if (share->nof_philo == 1)
		case_solo(share, status);
	else if (share->nof_philo % 2 != 0)
		case_oddphilos(share, status, 1,
			((share->time_to_eat / 2) * 3) - share->time_to_sleep);
	else
		case_evenphilos(share, status, 1);
	return (NULL);
}
