/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 20:32:49 by tssaito           #+#    #+#             */
/*   Updated: 2025/02/12 18:03:14 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		s1_len;
	int		s2_len;
	char	*ans;

	if (!s1 || !s2)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	ans = (char *)malloc(sizeof(char) * (s1_len + s2_len + 1));
	if (!ans)
		return (NULL);
	ft_strlcpy(ans, s1, s1_len + 1);
	ft_strlcat(ans, s2, s1_len + s2_len + 1);
	return (ans);
}
