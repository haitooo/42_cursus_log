#include "minishell.h"

static int	ft_abs(int n)
{
	if (n < 0)
		return (-n);
	return (n);
}

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

void	ft_putstr_fd(char *s, int fd)
{
	if (s == NULL)
		return ;
	while (*s)
	{
		write(fd, s, 1);
		s++;
	}
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t				i;
	unsigned char		*copy_dest;
	const unsigned char	*copy_src;

	if (!dest && !src)
		return (NULL);
	copy_dest = (unsigned char *)dest;
	copy_src = (unsigned char *)src;
	i = 0;
	while (i < n)
	{
		copy_dest[i] = copy_src[i];
		i++;
	}
	return (dest);
}

