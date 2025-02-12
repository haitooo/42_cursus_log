/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_utils2_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 04:02:49 by haito             #+#    #+#             */
/*   Updated: 2025/02/13 04:04:02 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "checker_bonus.h"

char	*ft_strdup(const char *s)
{
	char	*dest;
	int		i;
	int		srclen;

	i = 0;
	srclen = ft_strlen(s);
	dest = (char *)malloc(sizeof(char) * (srclen + 1));
	if (dest == NULL)
		return (NULL);
	while (s[i] != '\0')
	{
		dest[i] = s[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strjoin(char *s1, const char *s2)
{
	size_t	total_len;
	char	*result;
	char	*p;

	total_len = ft_strlen(s1) + ft_strlen(s2) + 1;
	result = (char *)malloc(total_len + 1);
	if (!result)
		return (0);
	p = result;
	while (*s1)
		*p++ = *s1++;
	*p++ = ' ';
	while (*s2)
		*p++ = *s2++;
	*p = '\0';
	return (result);
}

void	if_error(char *str)
{
	ft_printf("Error\n");
	free(str);
	exit(1);
}

char	**sep_argv(int *argc, char **argv, int n, char *str)
{
	char	**nums;
	char	*all_str;

	while (++n < *argc)
	{
		if (ft_strlen(argv[n]) == 0)
			if_error(str);
		if (n == 0)
		{
			str = ft_strdup(argv[n]);
			if (!str)
				if_error(str);
		}
		else
		{
			all_str = ft_strjoin(str, argv[n]);
			if (!all_str)
				if_error(str);
			free(str);
			str = all_str;
		}
	}
	nums = ft_split(str, ' ');
	*argc = count_words(str, ' ');
	return (free(str), nums);
}
