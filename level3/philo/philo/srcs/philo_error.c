/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hito <hito@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 10:47:24 by haito             #+#    #+#             */
/*   Updated: 2025/04/07 06:58:12 by hito             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	error_mutex_init(void)
{
	write(2, "philo: init_mutex failed\n", 25);
}

void	error_malloc(void)
{
	write(2, "phiro: malloc failed\n", 21);
}

void	error_invalid_arg(void)
{
	write(2, "philo: usage: ", 14);
	write(2, "./philo <numof_philo> ", 22);
	write(2, "<die> <eat> <sleep> <(optional)numof_must_eat>\n", 47);
}
