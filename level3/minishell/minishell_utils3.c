/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_utils3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 22:58:48 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/22 08:02:24 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strndup(const char *s, int n)
{
	char	*dest;
	int		i;

	i = 0;
	dest = (char *)malloc(sizeof(char) * (n + 1));
	if (!dest)
		return (NULL);
	while (s[i] != '\0' && i < n)
	{
		dest[i] = s[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	char	*dest;
	size_t	sizeof_str;

	sizeof_str = ft_strlen(s);
	if (start >= sizeof_str)
		return (ft_strdup(""));
	if (start + len > sizeof_str)
		len = sizeof_str - start;
	dest = (char *)malloc(len + 1);
	if (!dest)
		return (NULL);
	ft_strncpy(dest, s + start, len);
	dest[len] = '\0';
	return (dest);
}

void	ft_strcpy(char *new_str, char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		new_str[i] = str[i];
		i++;
	}
	new_str[i] = '\0';
}

char	*add_char(char *cmds, char c, int *i, t_var **var)
{
	size_t	len;
	char	*new_cmds;

	if (!cmds)
	{
		new_cmds = (char *)malloc(2);
		if (!new_cmds)
			return (update_exit_code(FAILED, var), error_add_char(i, NULL));
		new_cmds[0] = c;
		new_cmds[1] = '\0';
		return (new_cmds);
	}
	len = ft_strlen(cmds);
	new_cmds = (char *)malloc(len + 2);
	if (!new_cmds)
		return (update_exit_code(FAILED, var), error_add_char(i, cmds));
	ft_strcpy(new_cmds, cmds);
	new_cmds[len] = c;
	new_cmds[len + 1] = '\0';
	free(cmds);
	return (new_cmds);
}

void	ft_putendl_fd(char *s, int fd)
{
	if (!s)
		return ;
	while (*s)
	{
		write(fd, s, 1);
		s++;
	}
	write(fd, "\n", 1);
}
