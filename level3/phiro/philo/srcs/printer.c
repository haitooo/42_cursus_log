/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printer.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hito <hito@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 23:03:39 by hito              #+#    #+#             */
/*   Updated: 2025/04/08 00:10:33 by hito             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_messages(char **msgs, int count)
{
	while (--count >= 0)
		free(msgs[count]);
	free(msgs);
}

char	**init_message()
{
	char	**messages;

	messages = malloc(sizeof(char *) * 4);
	if (!messages)
		return (error_malloc(), NULL);
	messages[0] = ft_strdup("is eating\n");
	if (!messages[0])
		return (free_messages(messages, 0), error_malloc(), NULL);
	messages[1] = ft_strdup("is sleeping\n");
	if (!messages[1])
		return (free_messages(messages, 1), error_malloc(), NULL);
	messages[2] = ft_strdup("is thinking\n");
	if (!messages[2])
		return (free_messages(messages, 2), error_malloc(), NULL);
	messages[3] = ft_strdup("has taken a fork\n");
	if (!messages[3])
		return (free_messages(messages, 3), error_malloc(), NULL);
	return (messages);
}

void	*printer(void *arg)
{
	t_status	*statuses;
	t_share		*share;
	char		**messages;
	int			i;

	return (NULL);
	statuses = (t_status *)arg;
	share = statuses[0].share;
	messages = init_message();
	if (!messages)
	{
		pthread_mutex_lock(&share->m_print);
		share->someone_die = -1;
		pthread_mutex_unlock(&share->m_print);
		return (NULL);
	}
	while (share->start_flag != START && share->create_error == 0)
		usleep(50);
	while (1)
	{
		i = -1;
		while (++i < share->nof_philo)
		{
			pthread_mutex_lock(&share->m_print);
			if (share->someone_die != SURVIVED)
			{
				printf("%ld %d died\n", get_time_in_ms() - share->start_time + 1, share->someone_die);
				free_messages(messages, 4);
				return (NULL);
			}
			if (statuses[i].print_request != 0)
			{
				printf("%ld %d %s", get_time_in_ms() - share->start_time + 1, statuses[i].id, messages[statuses[i].print_request - 1]);
				statuses[i].print_request = 0;
			}
			pthread_mutex_unlock(&share->m_print);
		}
	}
}
