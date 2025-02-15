#include "ft_fprintf.h"

int	ft_fprint_addr(unsigned char *addr_hex)
{
	int		i;
	int		result;
	int		has_num;

	result = 0;
	has_num = 0;
	if (ft_fputstr("0x") == -1)
		return (-1);
	result += 2;
	i = -1;
	while (i++ < 15)
	{
		if (addr_hex[i] != '0' || has_num)
		{
			has_num = 1;
			if (ft_fputchar(addr_hex[i]) == -1)
				return (-1);
			result += 1;
		}
	}
	return (result);
}
