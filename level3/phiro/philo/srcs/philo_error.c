/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 10:47:24 by haito             #+#    #+#             */
/*   Updated: 2025/04/04 11:10:59 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	error_malloc(void)
{
	write(2, "phiro: malloc failed\n", 21);
}

void	error_invalid_arg(void)
{
	write(2, "philo: invalid arguments\n", 25);
}
