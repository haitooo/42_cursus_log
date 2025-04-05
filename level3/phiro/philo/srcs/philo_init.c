/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 10:42:00 by haito             #+#    #+#             */
/*   Updated: 2025/04/04 11:12:28 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_structs(t_share *share, int ac, char **av)
{
	share = malloc(sizeof(t_share));
	if (!share)
		return (error_malloc(), ERROR);
	share->start = 0;
	pthread_mutex_init(&share->m_start, NULL);
	share->nof_philo = ft_atoi(av[0]);
	share->time_to_die = ft_atoi(av[1]);
	share->time_to_eat = ft_atoi(av[2]);
	share->time_to_sleep = ft_atoi(av[3]);
	share->nof_must_eat = ft_atoi(av[4]);
	return (SUCCESS);
}
