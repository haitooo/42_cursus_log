#include <iostream>

int	main(int argc, char **argv)
{
	int	i;
	int	j;

	if (argc == 1)
		std::cout << "* LOUD AND UNBEARABLE FEEDBACK NOISE *" << std::endl;
	i = 0;
	while (++i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			std::cout << (char)std::to_upper
		}
	}
	return (0);
}
