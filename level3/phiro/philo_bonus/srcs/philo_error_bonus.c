/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_error_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 01:02:59 by haito             #+#    #+#             */
/*   Updated: 2025/06/24 04:43:21 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	error_invalid_arg(void)
{
	write(2, "philo: usage: ", 14);
	write(2, "./philo <numof_philo> ", 22);
	write(2, "<die> <eat> <sleep> <(optional)numof_must_eat>\n", 47);
}

void	error_malloc(void)
{
	write(2, "phiro: malloc failed\n", 21);
}

void	error_sem_open(void)
{
	write(2, "philo: sem_open failed\n", 23);
}
