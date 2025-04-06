/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hito <hito@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 10:42:00 by haito             #+#    #+#             */
/*   Updated: 2025/04/06 01:32:23 by hito             ###   ########.fr       */
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

int	init_structs(t_share **share, int ac, char **av)
{
	*share = malloc(sizeof(t_share));
	if (!*share)
		return (error_malloc(), ERROR);
	pthread_mutex_init(&(*share)->m_print, NULL);
	(*share)->start = 0;
	pthread_mutex_init(&(*share)->m_start, NULL);
	(*share)->nof_philo = ft_atoi(av[0]);
	(*share)->time_to_die = ft_atoi(av[1]);
	(*share)->time_to_eat = ft_atoi(av[2]);
	(*share)->time_to_sleep = ft_atoi(av[3]);
	if (ac == 5)
		(*share)->nof_must_eat = ft_atoi(av[4]);
	else
		(*share)->nof_must_eat = -1;
	if (check_overflow(*share) == INVALID)
		return (write(2, "philo: arg overflow\n", 20), free_structs(share), ERROR);
	return (SUCCESS);
}
