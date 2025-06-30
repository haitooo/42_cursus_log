/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   choose_case.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 12:40:35 by haito             #+#    #+#             */
/*   Updated: 2025/06/30 21:59:08 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	case_oddphilos(t_share *share, t_status *status, int is_first, long time)
{
	while (1)
	{
		if (!is_first && survival_check(share, status) == DIE)
			return (ERROR);
		if (status->id % 3 == 2)
			thinking(share, status, (share->time_to_eat / 2));
		if (status->id % 3 == 1 || status->id % 3 == 2)
		{
			if (routine_odd(share, status) == ERROR)
				return (ERROR);
			thinking(share, status, time);
		}
		if (status->id % 3 == 0)
		{
			if (is_first)
				thinking(share, status,
					(share->time_to_eat) + (share->time_to_eat / 2));
			else
				thinking(share, status, time);
			if (routine_odd(share, status) == ERROR)
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
			if (routine_even_(share, status) == ERROR)
				return (ERROR);
		}
		else
		{
			usleep(1000);
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

void	case_solo(t_share *share, t_status *status)
{
	if (share->nof_must_eat == 0)
	{
		pthread_mutex_lock(&share->m_nof_cleared);
		if (share->someone_die == 0)
		{
			share->someone_die = status->id;
			pthread_mutex_lock(&share->m_print);
			printf("%ld all philo avoid starving\n",
				get_time_in_ms() - share->start_time + 1);
			pthread_mutex_unlock(&share->m_print);
		}
		pthread_mutex_unlock(&share->m_nof_cleared);
	}
	else if (share->nof_philo == 1)
	{
		pthread_mutex_lock(&share->m_print);
		printf("%ld %d has taken a fork\n",
			get_time_in_ms() - share->start_time + 1, status->id);
		usleep(share->time_to_die * 1000);
		share->someone_die = status->id;
		printf("%ld %d died\n",
			get_time_in_ms() - share->start_time + 1, status->id);
		pthread_mutex_unlock(&share->m_print);
	}
}

void	after_eat(t_share *share, t_status *status)
{
	status->timeof_eaten++;
	pthread_mutex_lock(&share->m_nof_cleared);
	if (status->timeof_eaten == share->nof_must_eat)
		share->nof_cleared++;
	pthread_mutex_unlock(&share->m_nof_cleared);
}
