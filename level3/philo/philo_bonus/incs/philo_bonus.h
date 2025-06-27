/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 00:45:26 by haito             #+#    #+#             */
/*   Updated: 2025/06/27 20:10:29 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <limits.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <string.h>
# include <unistd.h>
# include <semaphore.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/time.h>

# define SLEEP 8000

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

typedef struct s_status
{
	int				id;
	long			last_meal;
	int				timeof_eaten;
	int				nof_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nof_must_eat;
	long			start_time;
}	t_status;

typedef struct s_sem
{
	sem_t		*sem_start;
	sem_t		*sem_fork;
	sem_t		*sem_print;
	sem_t		*sem_survival_check;
	sem_t		*sem_cleared;
}	t_sem;

// main
void	routine(t_status *status, t_sem *sem, pid_t **pids);
void	thinking(t_status *status, t_sem *sem, long time_to_think,
			pid_t **pids);
void	eating(t_status *status, t_sem *sem, pid_t **pids);
void	sleeping(t_status *status, t_sem *sem, pid_t **pids);
void	survival_check(t_status *status, t_sem *sem, pid_t **pids);
void	monitor(t_status *status, t_sem *sem, pid_t **pids);

// utils
bool	my_is_digit(int c);
int		ft_atoi(const char *str);
char	*ft_strdup(const char *s);
size_t	ft_strlen(const char *s);
long	get_time_in_ms(void);

// init
int		init_structs(t_status **status, int ac, char **av);
int		init_sem(t_sem **sem, t_status *status);

// free
void	free_sem(t_sem **sem, int errnum);
void	unlink_sem(void);

// error
void	error_invalid_arg(void);
void	error_malloc(void);
void	error_sem_open(void);

#endif