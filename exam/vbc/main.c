#include <stdio.h>
#include <ctype.h>

int	calculate_plus(char *input, int *position, int *has_error);

int	getnum_and_proceses_parents(char *input, int *position, int *has_error)
{
	char	current_char = input[*position];

	if	(isdigit(current_char))
	{
		(*position)++;
		if (isdigit(input[*position]))
		{
			*has_error = 1;
			return (-1);
		}
		return (current_char - '0');
	}
	if (current_char == '(')
	{
		(*position)++;
		int	value = calculate_plus(input, position, has_error);
		if (*has_error)
			return (-1);
		if (input[*position] != ')')
		{
			*has_error = 1;
			return (-1);
		}
		(*position)++;
		return (value);
	}
	*has_error = 1;
	return (-1);
}

int	calculate_multiply(char *input, int *position, int *has_error)
{
	int	value = getnum_and_proceses_parents(input, position, has_error);
	if (*has_error)
		return (-1);
	int	total = value;
	while (1)
	{
		if (input[*position] != '*')
			break ;
		(*position)++;
		value = getnum_and_proceses_parents(input, position, has_error);
		if (*has_error)
			return (-1);
		total *= value;
	}
	return (total);
}

int	calculate_plus(char *input, int *position, int *has_error)
{
	int	value = calculate_multiply(input, position, has_error);
	if (*has_error)
		return (-1);
	int	total = value;
	while (1)
	{
		if (input[*position] != '+')
			break ;
		(*position)++;
		value = calculate_multiply(input, position, has_error);
		if (*has_error)
			return (-1);
		total += value;
	}
	return (total);
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		return (1);
	char	*input = argv[1];
	int		position = 0;
	int		has_error = 0;
	int		result = calculate_plus(input, &position, &has_error);
	if ((!has_error && input[position] != '\0') || has_error)
		return (printf("Error\n"), 1);
	printf("%d\n", result);
	return (0);
}
