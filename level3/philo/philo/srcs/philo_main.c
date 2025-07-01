/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 01:35:10 by haito             #+#    #+#             */
/*   Updated: 2025/07/01 09:00:04 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_invalid_args(int argc, char **argv)
{
	int	i;
	int	j;

	i = 0;
	if (argc < 5 || argc > 6)
		return (error_invalid_arg(), INVALID);
	while (++i < argc)
	{
		j = -1;
		while (argv[i][++j])
		{
			if (!my_is_digit(argv[i][j]))
				return (error_invalid_arg(), INVALID);
		}
	}
	return (SUCCESS);
}

int	create_threads(t_share *share, pthread_t **threads,
		t_status *statuses, int i)
{
	*threads = malloc(sizeof(pthread_t) * share->nof_philo);
	if (!*threads)
		return (error_malloc(), ERROR);
	if (init_statuses(statuses, share) == ERROR)
		return (free(*threads), free(statuses), ERROR);
	pthread_mutex_lock(&share->m_start);
	while (++i < share->nof_philo)
	{
		if (pthread_create(&(*threads)[i], NULL, routine, &statuses[i]) != 0)
		{
			pthread_mutex_unlock(&share->m_start);
			while (--i >= 0)
				pthread_detach((*threads)[i]);
			share->create_error = ERROR;
			free(*threads);
			free_statuses(&statuses, share);
			return (write(2, "philo: thread_create failed\n", 28), ERROR);
		}
	}
	usleep(CREATE_WAITING);
	share->start_time = get_time_in_ms();
	share->start_flag = START;
	return (pthread_mutex_unlock(&share->m_start), SUCCESS);
}

int	join_threads(t_share **share, pthread_t **threads)
{
	int	i;

	i = -1;
	while (++i < (*share)->nof_philo)
		pthread_join((*threads)[i], NULL);
	free(*threads);
	return (SUCCESS);
}

int	main(int argc, char **argv)
{
	t_share		*share;
	pthread_t	*threads;
	t_status	*statuses;

	if (check_invalid_args(argc, argv) == INVALID)
		return (FAILED);
	if (init_structs(&share, --argc, ++argv) == ERROR)
		return (FAILED);
	statuses = malloc(sizeof(t_status) * share->nof_philo);
	if (!statuses)
		return (error_malloc(), free_share(&share, 0), ERROR);
	if (create_threads(share, &threads, statuses, -1) == ERROR)
		return (free_share(&share, 0), FAILED);
	join_threads(&share, &threads);
	free_statuses(&statuses, share);
	free_share(&share, 0);
	return (0);
}
