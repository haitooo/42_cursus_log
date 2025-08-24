/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 14:50:33 by tssaito           #+#    #+#             */
/*   Updated: 2025/02/12 18:02:21 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*copy_dest;
	const unsigned char	*copy_src;
	size_t				i;
	int					d;

	if (!dest && !src)
		return (NULL);
	copy_dest = (unsigned char *)dest;
	copy_src = (const unsigned char *)src;
	i = 0;
	d = 1;
	if (dest > src)
	{
		i = n - 1;
		d = -1;
	}
	while (n)
	{
		n--;
		copy_dest[i] = copy_src[i];
		i += d;
	}
	return (dest);
}
