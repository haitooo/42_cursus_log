/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 10:42:00 by haito             #+#    #+#             */
/*   Updated: 2025/06/20 20:27:01 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_overflow(t_share *share)
{
	if (share->nof_philo <= 0)
		return (INVALID);
	if (share->time_to_eat <= 0)
		return (INVALID);
	if (share->time_to_die <= 0)
		return (INVALID);
	if (share->time_to_sleep <= 0)
		return (INVALID);
	return (SUCCESS);
}

int	init_mutexes(t_share *share)
{
	int	i;

	share->m_fork = malloc(sizeof(pthread_mutex_t) * share->nof_philo);
	if (!share->m_fork)
		return (error_malloc(), ERROR);
	i = -1;
	while (++i < share->nof_philo)
	{
		if (pthread_mutex_init(&share->m_fork[i], NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&share->m_fork[i]);
			free(share->m_fork);
			return (error_mutex_init(), ERROR);
		}
	}
	if (pthread_mutex_init(&share->m_print, NULL) != 0)
		return (error_mutex_init(), free_share(&share, 1), ERROR);
	if (pthread_mutex_init(&share->m_start, NULL) != 0)
		return (error_mutex_init(), free_share(&share, 2), ERROR);
	if (pthread_mutex_init(&share->m_survival_check, NULL) != 0)
		return (error_mutex_init(), free_share(&share, 3), ERROR);
	if (pthread_mutex_init(&share->m_nof_cleared, NULL) != 0)
		return (error_mutex_init(), free_share(&share, 4), ERROR);
	return (SUCCESS);
}

int	init_structs(t_share **share, int ac, char **av)
{
	*share = malloc(sizeof(t_share));
	if (!*share)
		return (error_malloc(), ERROR);
	(*share)->start_flag = 0;
	(*share)->create_error = 0;
	(*share)->someone_die = 0;
	(*share)->nof_cleared = 0;
	(*share)->nof_philo = ft_atoi(av[0]);
	(*share)->time_to_die = ft_atoi(av[1]);
	(*share)->time_to_eat = ft_atoi(av[2]);
	(*share)->time_to_sleep = ft_atoi(av[3]);
	if (ac == 5)
		(*share)->nof_must_eat = ft_atoi(av[4]);
	else
		(*share)->nof_must_eat = -1;
	if (check_overflow(*share) == INVALID)
		return (write(2, "philo: arg overflow\n", 20), free(*share), ERROR);
	if (init_mutexes(*share) == ERROR)
		return (ERROR);
	return (SUCCESS);
}

int	init_statuses(t_status *statuses, t_share *share)
{
	int	i;

	i = -1;
	while (++i < share->nof_philo)
	{
		statuses[i].id = i + 1;
		statuses[i].share = share;
		statuses[i].last_meal = 0;
		statuses[i].timeof_eaten = 0;
		statuses[i].clear = 0;
		statuses[i].print_request = 0;
		if (pthread_mutex_init(&statuses[i].m_last_meal, NULL) != 0
			|| pthread_mutex_init(&statuses[i].m_timeof_eaten, NULL) != 0)
		{
			while (--i >= 0)
			{
				pthread_mutex_destroy(&statuses[i].m_last_meal);
				pthread_mutex_destroy(&statuses[i].m_timeof_eaten);
			}
			return (ERROR);
		}
	}
	return (SUCCESS);
}
