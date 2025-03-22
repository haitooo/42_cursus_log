/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 20:33:16 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/21 21:30:23 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	concat_files(char *dirname, t_wild **files)
{
	t_wild	*head;
	char	*totalname;
	int		len;

	if (!files)
		return ;
	head = *files;
	while (head)
	{
		len = ft_strlen(dirname) + ft_strlen(head->name) + 2;
		totalname = (char *)malloc(sizeof(char) * len);
		if (!totalname)
			return ;
		ft_strlcpy(totalname, dirname, len);
		ft_strlcat(totalname, "/", len);
		ft_strlcat(totalname, head->name, len);
		free(head->name);
		head->name = totalname;
		head = head->next;
	}
}

int	check_wildcard(char *str)
{
	int		i;
	char	ope;

	i = 0;
	while (str[i])
	{
		if (str[i] == '*')
			return (1);
		else if (str[i] == '\'' || str[i] == '\"')
		{
			ope = str[i];
			i++;
			while (str[i] != ope)
				i++;
			i++;
		}
		else
			i++;
	}
	return (0);
}

void	connect_to_the_end(t_wild **files, t_wild **new_files)
{
	t_wild	*head;

	head = *files;
	while (head && head->next)
		head = head->next;
	head->next = *new_files;
}
