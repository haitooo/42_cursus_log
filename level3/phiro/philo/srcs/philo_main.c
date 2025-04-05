/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 01:35:10 by haito             #+#    #+#             */
/*   Updated: 2025/04/04 11:12:42 by haito            ###   ########.fr       */
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

int	main(int argc, char **argv)
{
	t_share	share;

	if (check_invalid_args(argc, argv) == INVALID)
		return (FAILED);
	if (init_structs(&share, --argc, ++argv) == ERROR)
		return (FAILED);
	//create_thread(&share);
	//pthread_mutex_lock(&share.m_start);
	//share.start = 1;
	//pthread_mutex_unlock(&share.m_start);
	//join_thread();
	return (0);
}
