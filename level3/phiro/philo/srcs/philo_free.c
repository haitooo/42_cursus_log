/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 10:43:48 by haito             #+#    #+#             */
/*   Updated: 2025/04/06 18:36:01 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_structs(t_share **share, int errnum)
{
	int	i;

	i = -1;
	while (++i < (*share)->nof_philo)
		pthread_mutex_destroy(&(*share)->m_fork[i]);
	free((*share)->m_fork);
	if (errnum >= 2 || errnum == 0)
		pthread_mutex_destroy(&(*share)->m_print);
	if (errnum >= 3 || errnum == 0)
		pthread_mutex_destroy(&(*share)->m_start);
	free(*share);
}
