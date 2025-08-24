/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 21:27:52 by tssaito           #+#    #+#             */
/*   Updated: 2025/02/12 18:03:48 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	unsigned int	s_len;
	char			*ans;

	if (!s)
		return (NULL);
	s_len = ft_strlen(s);
	if (s_len < start)
		len = 0;
	else if (s_len - start < len)
		len = s_len - start;
	ans = (char *)malloc(sizeof(char) * len + 1);
	if (!ans)
		return (NULL);
	if (!len || s_len < start)
		ft_strlcpy(ans, s, 1);
	else
		ft_strlcpy(ans, s + start, len + 1);
	return (ans);
}
