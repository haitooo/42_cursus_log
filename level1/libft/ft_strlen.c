/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 18:31:36 by haito             #+#    #+#             */
/*   Updated: 2024/10/22 18:56:19 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlen(const char *s)
{
	int	count;

	count = 0;
	while (s[count])
		count++;
	return (count);
}

// int	main(void)
// {
// 	const char	s[50] = "abcde";
// 	int			result;

// 	result = ft_strlen(s);
// 	printf("%d\n", result);
// 	return (0);
// }
