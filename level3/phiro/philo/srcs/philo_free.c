/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hito <hito@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 10:43:48 by haito             #+#    #+#             */
/*   Updated: 2025/04/07 22:31:48 by hito             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_share(t_share **share, int errnum)
{
	int	i;

	if (!share || !*share)
		return ;
	i = -1;
	while (++i < (*share)->nof_philo)
		pthread_mutex_destroy(&(*share)->m_fork[i]);
	free((*share)->m_fork);
	if (errnum >= 2 || errnum == 0)
		pthread_mutex_destroy(&(*share)->m_print);
	if (errnum >= 3 || errnum == 0)
		pthread_mutex_destroy(&(*share)->m_start);
	if (errnum >= 4 || errnum == 0)
		pthread_mutex_destroy(&(*share)->m_survival_check);
	free(*share);
	*share = NULL;
}

void	free_statuses(t_status **statuses, t_share *share)
{
	int	i;

	if (!statuses || !*statuses)
		return ;
	i = -1;
	while (++i < share->nof_philo)
		pthread_mutex_destroy(&(*statuses)[i].m_last_meal);
	free(*statuses);
	*statuses = NULL;
}
