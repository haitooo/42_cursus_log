/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 23:00:18 by tssaito           #+#    #+#             */
/*   Updated: 2025/02/12 18:02:11 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*copy_s1;
	unsigned char	*copy_s2;
	size_t			i;

	copy_s1 = (unsigned char *)s1;
	copy_s2 = (unsigned char *)s2;
	i = 0;
	while (n)
	{
		if (copy_s1[i] != copy_s2[i])
			return (copy_s1[i] - copy_s2[i]);
		n--;
		i++;
	}
	return (0);
}
