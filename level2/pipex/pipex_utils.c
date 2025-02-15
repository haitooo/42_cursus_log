#include "pipex.h"

size_t	ft_strlen(const char *s)
{
	int	count;

	count = 0;
	while (s && s[count])
		count++;
	return (count);
}

int	ft_strlen_sep(const char *str, const char c)
{
	int	count;

	count = 0;
	while (str[count] != c && str[count])
		count++;
	return (count);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	count;

	count = 0;
	while (s1[count] && s2[count])
	{
		if (s1[count] != s2[count])
			return ((unsigned char)s1[count] - (unsigned char)s2[count]);
		count++;
	}
	return ((unsigned char)s1[count] - (unsigned char)s2[count]);
}

int	ft_strncmp(const char *s1, const char *s2, int n)
{
	int	count;

	count = 0;
	while (count < n && s1[count] && s2[count])
	{
		if (s1[count] != s2[count])
			return ((unsigned char)s1[count] - (unsigned char)s2[count]);
		count++;
	}
	if (count == n)
		return (0);
	return ((unsigned char)s1[count] - (unsigned char)s2[count]);
}
