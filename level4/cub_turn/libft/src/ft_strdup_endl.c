/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup_endl.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 17:22:23 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/06 13:10:38 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup_endl(const char *s)
{
	int		i;
	char	*ans;

	if (!s)
		return (NULL);
	ans = (char *)malloc(sizeof(char) * ft_strlen(s) + 2);
	if (!ans)
		return (NULL);
	i = 0;
	while (s[i])
	{
		ans[i] = s[i];
		i++;
	}
	ans[i++] = '\n';
	ans[i] = '\0';
	return (ans);
}
