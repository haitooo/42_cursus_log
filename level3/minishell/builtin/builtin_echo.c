/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 20:14:18 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/22 04:27:22 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_option(char *str)
{
	int	i;

	if (str[0] != '-' || str[1] != 'n')
		return (0);
	i = 2;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

static size_t	count_size(t_tokens **tokens, int option)
{
	size_t		malloc_size;
	t_tokens	*head;

	malloc_size = 0;
	head = *tokens;
	head = head->next;
	while (head && check_option(head->token))
		head = head->next;
	while (head)
	{
		malloc_size += ft_strlen(head->token);
		head = head->next;
		if (head)
			malloc_size++;
	}
	if (!option)
		malloc_size++;
	return (malloc_size);
}

static char	*make_total_str(t_tokens **tokens, int malloc_size, int option)
{
	char		*total_str;
	t_tokens	*head;

	head = *tokens;
	head = head->next;
	total_str = (char *)malloc(sizeof(char) * malloc_size);
	if (!total_str)
		return (NULL);
	while (head && check_option(head->token))
		head = head->next;
	total_str[0] = '\0';
	while (head)
	{
		if (head->token[0])
			ft_strlcat(total_str, head->token, malloc_size);
		head = head->next;
		if (head)
			ft_strlcat(total_str, " ", malloc_size);
	}
	if (!option)
		ft_strlcat(total_str, "\n", malloc_size);
	return (total_str);
}

int	builtin_echo(t_tokens **tokens, t_var **varlist)
{
	int			is_option;
	size_t		malloc_size;
	char		*total_str;
	t_tokens	*head;

	(void)varlist;
	head = *tokens;
	if (head && head->next)
	{
		is_option = check_option(head->next->token);
		malloc_size = count_size(tokens, is_option) + 1;
		total_str = make_total_str(tokens, malloc_size, is_option);
		if (!total_str)
			return (EXIT_FAILURE);
		ft_putstr_fd(total_str, STDOUT_FILENO);
		free(total_str);
	}
	else
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
