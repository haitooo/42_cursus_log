/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 21:06:38 by tssaito           #+#    #+#             */
/*   Updated: 2025/07/03 22:12:16 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*ans;
	int		start;
	int		end;
	int		len;

	if (!s1 || !set)
		return (NULL);
	len = ft_strlen(s1);
	start = 0;
	while (start < len && ft_strchr(set, s1[start]))
		start++;
	end = len;
	while (end > start && end - 1 >= 0 && ft_strchr(set, s1[end - 1]))
		end--;
	if (start >= end)
		ans = (char *)malloc(sizeof(char) * 1);
	else
		ans = (char *)malloc(sizeof(char) * (end - start + 1));
	if (!ans)
		return (NULL);
	if (start >= end)
		ans[0] = '\0';
	else
		ft_strlcpy(ans, &s1[start], end - start + 1);
	return (ans);
}
