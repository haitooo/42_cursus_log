/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 01:35:10 by haito             #+#    #+#             */
/*   Updated: 2025/04/06 22:36:23 by haito            ###   ########.fr       */
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
	while (++i < argc - 1)
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

int	create_threads(t_share *share, pthread_t **threads, t_status *philos)
{
	int			i;

	*threads = malloc(sizeof(pthread_t) * share->nof_philo);
	if (!*threads)
		return (error_malloc(), ERROR);
	i = -1;
	while (++i < share->nof_philo)
	{
		philos[i].id = i + 1;
		philos[i].share = share;
		if (pthread_create(&(*threads)[i], NULL, routine, &philos[i]) != 0)
			return (write(2, "philo: thread_create failed\n", 28), ERROR);
		usleep(100);
		share->start = 1;
	}
	return (SUCCESS);
}

int	join_threads(t_share **share, pthread_t **threads)
{
	int	i;

	i = -1;
	while (++i < (*share)->nof_philo)
		pthread_join((*threads)[i], NULL);
	free(*threads);
	return (0);
}

int	main(int argc, char **argv)
{
	t_share		*share;
	pthread_t	*threads;
	t_status	*philos;

	if (check_invalid_args(argc, argv) == INVALID)
		return (FAILED);
	if (init_structs(&share, --argc, ++argv) == ERROR)
		return (FAILED);
	philos = malloc(sizeof(t_status) * share->nof_philo);
	if (!philos)
		return (error_malloc(), ERROR);
	if (create_threads(share, &threads, philos) == ERROR)
		return (FAILED);
	join_threads(&share, &threads);
	free_structs(&share, 0);
	free(philos);
	return (0);
}
