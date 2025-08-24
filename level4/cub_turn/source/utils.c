/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:29:27 by tssaito           #+#    #+#             */
/*   Updated: 2025/08/24 18:00:33 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// Frees the memory allocated for a 2D array of strings (char **).
// It iterates through each string, frees it, and then frees the array itself.
void	ft_free_split(char **split)
{
	int	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(split);
}

// Reallocates a 2D string array to add a new string.
// It creates a new, larger array, copies the pointers from the old array,
// adds the new string, null-terminates the new array, and frees the old array.
char	**ft_realloc_2d_array(char **arr, int old_size, char *new_str)
{
	char	**new_arr;
	int		i;

	new_arr = malloc(sizeof(char *) * (old_size + 2));
	if (!new_arr)
	{
		ft_free_split(arr);
		return (NULL);
	}
	i = 0;
	while (i < old_size)
	{
		new_arr[i] = arr[i];
		i++;
	}
	new_arr[old_size] = new_str;
	new_arr[old_size + 1] = NULL;
	if (arr)
		free(arr);
	return (new_arr);
}

static int	ft_strrncmp(const char *s1, const char *s2, size_t n)
{
	size_t	s1_len;
	size_t	s2_len;

	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	if (s1_len < n || s2_len < n)
		return (1);
	return (ft_strncmp(s1 + s1_len - n, s2, n));
}

void	check_filename(const char *filename)
{
	if (!filename || ft_strrncmp(filename, ".cub", 4) != 0)
		exit_error("Invalid file extension. Must be .cub", NULL);
}
