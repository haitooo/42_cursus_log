/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 20:51:14 by haito             #+#    #+#             */
/*   Updated: 2024/10/25 20:55:13 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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

// int	main(void)
// {
// 	char str[] = "abc123";
// 	char *dest;

// 	dest = ft_strdup(str);
// 	printf("%s\n", dest);
// 	free(dest);
// 	return (0);
// }
