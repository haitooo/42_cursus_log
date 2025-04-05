/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 01:36:21 by haito             #+#    #+#             */
/*   Updated: 2025/04/04 11:11:05 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <string.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef enum e_result
{
	SUCCESS = 0,
	FAILED = 1,
	ERROR = -1,
	INVALID = 1,
}	t_result;

typedef struct s_share
{
	int				start;
	pthread_mutex_t	*m_start;
	int				nof_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nof_must_eat;
}	t_share;

bool	my_is_digit(int c);
int		ft_atoi(const char *str);
int		init_structs(t_share *share, int ac, char **av);
void	free_structs(t_share *share);
void	error_malloc(void);
void	error_invalid_arg(void);

#endif