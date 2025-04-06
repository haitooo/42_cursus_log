/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 10:42:00 by haito             #+#    #+#             */
/*   Updated: 2025/04/06 18:35:54 by haito            ###   ########.fr       */
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
		return (error_mutex_init(), free_structs(&share, 1), ERROR);
	if (pthread_mutex_init(&share->m_start, NULL) != 0)
		return (error_mutex_init(), free_structs(&share, 2), ERROR);
	return (SUCCESS);
}

int	init_structs(t_share **share, int ac, char **av)
{
	*share = malloc(sizeof(t_share));
	if (!*share)
		return (error_malloc(), ERROR);
	(*share)->start = 0;
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
