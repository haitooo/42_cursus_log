/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_free_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 04:37:36 by haito             #+#    #+#             */
/*   Updated: 2025/06/27 15:52:23 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	unlink_sem(void)
{
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/start");
	sem_unlink("/scheck");
	sem_unlink("/cleared");
	sem_unlink("/check");
}

void	free_sem(t_sem **sem, int errnum)
{
	sem_close((*sem)->sem_fork);
	sem_unlink("/forks");
	if (errnum < 1)
	{
		sem_close((*sem)->sem_print);
		sem_unlink("/print");
	}
	if (errnum < 2)
	{
		sem_close((*sem)->sem_start);
		sem_unlink("/start");
	}
	if (errnum < 3)
	{
		sem_close((*sem)->sem_survival_check);
		sem_unlink("/scheck");
	}
	if (errnum < 4)
	{
		sem_close((*sem)->sem_cleared);
		sem_unlink("/cleared");
	}
	free(*sem);
}
