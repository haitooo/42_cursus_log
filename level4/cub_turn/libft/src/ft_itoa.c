/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 12:27:13 by tssaito           #+#    #+#             */
/*   Updated: 2025/02/12 19:46:21 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_num(int n)
{
	int	count;

	count = 0;
	if (n <= 0)
		count++;
	while (n)
	{
		count++;
		n /= 10;
	}
	return (count);
}

static int	ft_abs(int n)
{
	if (n < 0)
		return (-n);
	return (n);
}

char	*ft_itoa(int n)
{
	char	*ans;
	int		len;
	int		sign;

	len = count_num(n);
	ans = (char *)malloc(sizeof(char) * len + 1);
	if (!ans)
		return (NULL);
	sign = 1;
	if (n < 0)
		sign = -1;
	*(ans + len) = '\0';
	len--;
	while (len >= 0)
	{
		*(ans + len) = ft_abs(n % 10) + '0';
		n /= 10;
		len--;
	}
	if (sign < 0)
		*ans = '-';
	return (ans);
}
