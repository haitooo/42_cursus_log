/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 19:29:27 by tssaito           #+#    #+#             */
/*   Updated: 2025/08/24 11:41:11 by tssaito          ###   ########.fr       */
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
