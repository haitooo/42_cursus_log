/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 17:34:23 by tssaito           #+#    #+#             */
/*   Updated: 2025/02/12 18:03:18 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dsize)
{
	size_t	dst_len;
	size_t	src_len;

	src_len = ft_strlen(src);
	if (!dsize)
		return (src_len);
	dst_len = ft_strlen(dst);
	if (dsize <= dst_len)
		return (dsize + src_len);
	ft_strlcpy(dst + dst_len, src, dsize - dst_len);
	return (dst_len + src_len);
}
