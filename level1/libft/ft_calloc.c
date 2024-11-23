/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/25 20:14:47 by haito             #+#    #+#             */
/*   Updated: 2024/10/26 05:21:21 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	char	*dest;
	size_t	total_size;

	if (nmemb != 0 && size > SIZE_MAX / nmemb)
		return (0);
	total_size = nmemb * size;
	if (nmemb <= 0 || size <= 0)
	{
		dest = (void *)malloc(0);
		return (dest);
	}
	dest = (void *)malloc(total_size);
	if (!dest)
		return (0);
	ft_memset(dest, '\0', total_size);
	return (dest);
}

// int	main(void)
// {
// 	char	*p;
// 	char	*p_;
// 	size_t	i;
// 	size_t	n = 2222;

// 	p = (char *)ft_calloc(n, sizeof(char));
// 	p_ = (char *)calloc(n, sizeof(char));

// 	i = 0;
// 	while (i < n)
// 	{
// 		printf("p[%zu] = %d\n", i, p[i]);
// 		i++;
// 	}
// 	i = 0;
// 	while (i < n)
// 	{
// 		printf("p_[%zu] = %d\n", i, p_[i]);
// 		i++;
// 	}
// 	free(p);
// 	free(p_);
// }
