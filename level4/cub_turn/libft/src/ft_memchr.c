/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 18:20:51 by tssaito           #+#    #+#             */
/*   Updated: 2025/02/12 18:02:05 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*copy_s;
	unsigned char	copy_c;

	copy_s = (unsigned char *)s;
	copy_c = (unsigned char)c;
	while (n)
	{
		if (*copy_s == copy_c)
			return ((void *)copy_s);
		copy_s++;
		n--;
	}
	return (NULL);
}
