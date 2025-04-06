/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hito <hito@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 01:34:03 by hito              #+#    #+#             */
/*   Updated: 2025/04/06 01:34:20 by hito             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*routine(void *arg)
{
	t_share *share;

	share = (t_share *)arg;
	pthread_mutex_lock(&share->m_print);
	printf("share:%d\n", share->nof_philo);
	pthread_mutex_unlock(&share->m_print);
	return (NULL);
}
