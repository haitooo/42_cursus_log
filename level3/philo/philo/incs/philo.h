/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 01:36:21 by haito             #+#    #+#             */
/*   Updated: 2025/06/30 21:59:53 by haito            ###   ########.fr       */
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

# define START 1
# define CREATE_WAITING 1000000
# define SLEEP 13000

typedef enum e_result
{
	SUCCESS = 0,
	FAILED = 1,
	ERROR = -1,
	INVALID = 1,
	DIE = 1,
	CLEAR = 1,
	SURVIVED = 0,
}	t_result;

typedef struct s_share
{
	int				start_flag;
	int				create_error;
	long			start_time;
	int				someone_die;
	int				nof_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nof_must_eat;
	int				nof_cleared;
	pthread_mutex_t	m_print;
	pthread_mutex_t	m_start;
	pthread_mutex_t	m_survival_check;
	pthread_mutex_t	m_nof_cleared;
	pthread_mutex_t	*m_fork;
}	t_share;

typedef struct s_status
{
	int				id;
	long			last_meal;
	int				timeof_eaten;
	pthread_mutex_t	*my_fork_r;
	pthread_mutex_t	*my_fork_l;
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;
	pthread_mutex_t	m_last_meal;
	t_share			*share;
}	t_status;

bool	my_is_digit(int c);
int		ft_atoi(const char *str);
char	*ft_strdup(const char *s);
size_t	ft_strlen(const char *s);
int		init_structs(t_share **share, int ac, char **av);
int		init_statuses(t_status *statuses, t_share *share);
void	init_forks(t_share *share, t_status *status);
void	free_share(t_share **share, int errnum);
void	free_statuses(t_status **statuses, t_share *share);
void	error_malloc(void);
void	error_mutex_init(void);
void	error_invalid_arg(void);

void	*routine(void *arg);
int		routine_even(t_share *share, t_status *status);
int		routine_even_(t_share *share, t_status *status);
int		routine_odd(t_share *share, t_status *status);
void	case_solo(t_share *share, t_status *status);
int		case_evenphilos(t_share *share, t_status *status, int is_first);
int		case_oddphilos(t_share *share, t_status *status, int is_first,
			long time);
void	after_eat(t_share *share, t_status *status);
int		thinking(t_share *share, t_status *status, long time_to_think);
int		eating(t_share *share, t_status *status, long time);
int		sleeping(t_share *share, t_status *status);
int		survival_check(t_share *share, t_status *status);
long	get_time_in_ms(void);

#endif