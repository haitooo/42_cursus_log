/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 01:36:21 by haito             #+#    #+#             */
/*   Updated: 2025/04/06 22:25:25 by haito            ###   ########.fr       */
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
	int				nof_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nof_must_eat;
	pthread_mutex_t	m_print;
	pthread_mutex_t	m_start;
	pthread_mutex_t	*m_fork;
}	t_share;

typedef struct s_status
{
	int				id;
	t_share			*share;
	pthread_mutex_t	*my_fork_r;
	pthread_mutex_t	*my_fork_l;
}	t_status;

bool	my_is_digit(int c);
int		ft_atoi(const char *str);
int		init_structs(t_share **share, int ac, char **av);
void	free_structs(t_share **share, int errnum);
void	error_malloc(void);
void	error_mutex_init(void);
void	error_invalid_arg(void);

void	*routine(void *arg);
long	get_time_in_ms(void);

#endif