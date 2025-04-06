/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hito <hito@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 01:35:10 by haito             #+#    #+#             */
/*   Updated: 2025/04/06 01:33:53 by hito             ###   ########.fr       */
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

int	create_threads(t_share **share, pthread_t **threads)
{
	int	i;

	*threads = malloc(sizeof(pthread_t) * (*share)->nof_philo);
	if (!*threads)
		return (error_malloc(), ERROR);
	i = -1;
	while (++i < (*share)->nof_philo)
		pthread_create(&(*threads)[i], NULL, routine, *share);
	return (0);
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

	if (check_invalid_args(argc, argv) == INVALID)
		return (FAILED);
	if (init_structs(&share, --argc, ++argv) == ERROR)
		return (FAILED);
	// printf("%d\n", share->nof_philo);
	// printf("%d\n", share->time_to_die);
	// printf("%d\n", share->time_to_eat);
	// printf("%d\n", share->time_to_sleep);
	// printf("%d\n", share->nof_must_eat);
	if (create_threads(&share, &threads) == ERROR)
		return (FAILED);
	//pthread_mutex_lock(&share.m_start);
	//share.start = 1;
	//pthread_mutex_unlock(&share.m_start);
	join_threads(&share, &threads);
	free_structs(&share);
	return (0);
}
