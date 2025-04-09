/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hito <hito@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 17:00:41 by hito              #+#    #+#             */
/*   Updated: 2025/04/07 19:00:24 by hito             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*monitoring(void *arg)
{
	t_status	*statuses;
	t_share		*share;

	statuses = (t_status *)arg;
	share = statuses[0].share;
	while (share->create_error == 0 && share->start_flag != START)
		usleep(50);
	if (share->create_error == ERROR)
		return (NULL);
}
