/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 02:29:16 by haito             #+#    #+#             */
/*   Updated: 2024/10/26 05:24:05 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	size_t			sizeof_str;
	unsigned int	i;

	if (!s || !f)
		return ;
	sizeof_str = ft_strlen(s);
	i = 0;
	while (i < sizeof_str)
	{
		f(i, &s[i]);
		i++;
	}
	return ;
}

// void	ft_upper_a(unsigned int i, char *c)
// {
// 	(void)i;
// 	if (*c >= 'a' && *c <= 'z')
// 		*c -= 32;
// 	return ;
// }

// int	main(void)
// {
// 	char	str[] = "abc4def";

// 	printf("before:%s\n", str);
// 	ft_striteri(str, ft_upper_a);
// 	printf("after:%s\n", str);
// 	return (0);
// }
