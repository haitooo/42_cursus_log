/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 01:51:30 by haito             #+#    #+#             */
/*   Updated: 2024/10/26 02:16:07 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	get_digits(int n)
{
	size_t	digits;

	digits = 0;
	while (n)
	{
		n /= 10;
		digits++;
	}
	return (digits);
}

static char	*is_zero(void)
{
	char	*zero;

	zero = (char *)malloc(2);
	if (!zero)
		return (0);
	if (zero)
	{
		zero[0] = '0';
		zero[1] = '\0';
	}
	return (zero);
}

static char	*is_min(void)
{
	char	*min;

	min = (char *)malloc(12);
	if (!min)
		return (0);
	if (min)
	{
		min[0] = '-';
		min[1] = '2';
		min[2] = '1';
		min[3] = '4';
		min[4] = '7';
		min[5] = '4';
		min[6] = '8';
		min[7] = '3';
		min[8] = '6';
		min[9] = '4';
		min[10] = '8';
		min[11] = '\0';
	}
	return (min);
}

char	*ft_itoa(int n)
{
	size_t	digits;
	size_t	is_nega;
	char	*result;

	is_nega = 0;
	if (n == 0)
		return (is_zero());
	if (n == -2147483648)
		return (is_min());
	is_nega = (n < 0);
	if (is_nega)
		n = -n;
	digits = get_digits(n) + is_nega;
	result = (char *)malloc(digits + 1);
	if (!result)
		return (0);
	result[digits] = '\0';
	if (is_nega)
		result[0] = '-';
	while (digits-- > is_nega)
	{
		result[digits] = n % 10 + '0';
		n /= 10;
	}
	return (result);
}

// int	main(void)
// {
// 	char *str;

// 	str = ft_itoa(123);
// 	printf("%s\n", str);
// 	free(str);

// 	str = ft_itoa(-123);
// 	printf("%s\n", str);
// 	free(str);

// 	str = ft_itoa(0);
// 	printf("%s\n", str);
// 	free(str);

// 	str = ft_itoa(-2147483648);
// 	printf("%s\n", str);
// 	free(str);

// 	return 0;
// }
