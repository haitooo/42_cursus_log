/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_make.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 00:53:59 by haito             #+#    #+#             */
/*   Updated: 2025/02/22 09:50:08 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

char	*add_char(char *line, char c)
{
	char	*new_line;
	int		length;
	int		n;

	n = -1;
	if (!line)
		return (NULL);
	length = ft_strlen(line);
	new_line = (char *)malloc(length + 2);
	if (!new_line)
		return (NULL);
	while (++n < length)
		new_line[n] = line[n];
	new_line[n++] = c;
	new_line[n] = '\0';
	free(line);
	line = NULL;
	return (new_line);
}

char	*read_from_file(int fd)
{
	int		bytes_read;
	char	buf[BUFFER_SIZE + 1];
	int		n;
	char	*line;

	line = malloc(sizeof(char));
	if (!line)
		return (NULL);
	line[0] = '\0';
	while (1)
	{
		n = 0;
		bytes_read = read(fd, buf, BUFFER_SIZE);
		if (bytes_read < 0)
			return (free(line), NULL);
		if (bytes_read == 0)
			break ;
		while (n < bytes_read)
		{
			line = add_char(line, buf[n++]);
			if (!line)
				return (NULL);
		}
	}
	return (line);
}

char	**read_map(t_map *map, char *mapname)
{
	int		fd;
	char	*str;
	char	**objs;

	fd = open(mapname, O_RDONLY);
	if (fd < 0)
		return (NULL);
	str = read_from_file(fd);
	if (!str)
		return (NULL);
	map->map_height = count_words(str, '\n');
	objs = ft_split(str, '\n');
	map->map_width = ft_strlen(objs[0]);
	free(str);
	str = NULL;
	return (objs);
}

int	count_object(t_map *map, int n)
{
	int	i;

	i = -1;
	while (++i < map->map_width)
	{
		if (map->objs[n][i] == 'C')
			map->num_c++;
		if (map->objs[n][i] == 'E')
		{
			if (++map->num_e > 1)
				return (-1);
		}
		if (map->objs[n][i] == 'P')
		{
			map->x = i;
			map->y = n;
			if (++map->num_p > 1)
				return (-1);
		}
		if (map->objs[n][i] != 'P' && map->objs[n][i] != 'E'
			&& map->objs[n][i] != 'C' && map->objs[n][i] != '0'
				&& map->objs[n][i] != '1')
			return (-1);
	}
	return (0);
}

void	init_objs_count(t_map *map)
{
	map->num_c = 0;
	map->num_e = 0;
	map->num_p = 0;
}
