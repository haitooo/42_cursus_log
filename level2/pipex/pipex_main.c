/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:31:04 by haito             #+#    #+#             */
/*   Updated: 2025/02/13 19:32:33 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv)
{
	char	*infile_name;

	if (argc <= 4)
		return (write(1, "Error\n", 6), 1);
	// if (ft_strcmp(argv[1], "hre_doc") == 0)
	// 	get_from_stdin();
	// else
		infile_name = argv[1];
	
}
