/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 00:55:15 by haito             #+#    #+#             */
/*   Updated: 2025/06/27 16:25:36 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	check_overflow(t_status *status, int ac)
{
	if (status->nof_philo <= 0)
		return (INVALID);
	if (status->time_to_eat <= 0)
		return (INVALID);
	if (status->time_to_die <= 0)
		return (INVALID);
	if (status->time_to_sleep <= 0)
		return (INVALID);
	if (ac == 5 && status->nof_must_eat < 0)
		return (INVALID);
	return (SUCCESS);
}

int	init_structs(t_status **status, int ac, char **av)
{
	*status = malloc(sizeof(t_status));
	if (!*status)
		return (error_malloc(), ERROR);
	(*status)->nof_philo = ft_atoi(av[0]);
	(*status)->time_to_die = ft_atoi(av[1]);
	(*status)->time_to_eat = ft_atoi(av[2]);
	(*status)->time_to_sleep = ft_atoi(av[3]);
	if (ac == 5)
		(*status)->nof_must_eat = ft_atoi(av[4]);
	else
		(*status)->nof_must_eat = -1;
	(*status)->timeof_eaten = 0;
	if (check_overflow(*status, ac) == INVALID)
		return (write(2, "philo: arg overflow\n", 20), free(*status), ERROR);
	return (SUCCESS);
}

int	init_sem(t_sem **sem, t_status *status)
{
	(*sem) = malloc(sizeof(t_sem));
	if (!(*sem))
		return (error_malloc(), FAILED);
	(*sem)->sem_fork = sem_open("/forks", O_CREAT | O_EXCL,
			0644, status->nof_philo);
	if ((*sem)->sem_fork == SEM_FAILED)
		return (error_sem_open(), FAILED);
	(*sem)->sem_print = sem_open("/print", O_CREAT | O_EXCL, 0644, 1);
	if ((*sem)->sem_print == SEM_FAILED)
		return (free_sem(sem, 1), error_sem_open(), FAILED);
	(*sem)->sem_start = sem_open("/start", O_CREAT | O_EXCL, 0644, 0);
	if ((*sem)->sem_start == SEM_FAILED)
		return (free_sem(sem, 2), error_sem_open(), FAILED);
	(*sem)->sem_survival_check = sem_open("/scheck", O_CREAT | O_EXCL, 0644, 1);
	if ((*sem)->sem_survival_check == SEM_FAILED)
		return (free_sem(sem, 3), error_sem_open(), FAILED);
	(*sem)->sem_cleared = sem_open("/cleared", O_CREAT | O_EXCL, 0644, 0);
	if ((*sem)->sem_cleared == SEM_FAILED)
		return (free_sem(sem, 4), error_sem_open(), FAILED);
	return (SUCCESS);
}
