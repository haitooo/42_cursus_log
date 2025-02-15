/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haito <haito@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:31:07 by haito             #+#    #+#             */
/*   Updated: 2025/02/13 16:31:08 by haito            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

//# define ENV_PATH "/proc/self/environ"
# define BUF_SIZE 4096

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <sys/wait.h>
# include <stddef.h>
# include "ft_printf/ft_printf.h"
# include "ft_fprintf/ft_fprintf.h"
# include "gnl/get_next_line.h"

typedef struct s_cmd
{
	int		size_cmd;
	char	**cmds;
	char	**path;
	char	*infile_name;
	char	*outfile_name;
	char	*env_path;
}	t_cmd;

typedef struct s_param
{
	int	j;
	int	n;
	int	size;
	int	size_cmd_len;
}	t_param;

char	**get_path(t_cmd arg, char **envp);
void	free_pipefd(int **pipefd, int i);
//void	close_pipefd(int **pipefd, int i);
void	after_call_child(t_cmd arg, int **pipefd);
int		**init_pipefd(t_cmd arg);
pid_t	*init_pids(t_cmd arg, int **pipefd);
void	continue_child(int **pipefd, t_cmd arg, int i, char **envp);
void	execute_command(t_cmd arg, char **envp, int i);

void	error_fork(t_cmd arg, int **pipefd, pid_t *pids);
void	error_get_path(t_cmd arg);
char	**error_in_get_path(char **path, int errornum, int n);
void	error_init_pipefd(t_cmd arg, int i, int **pipefd, int errrornum);
void	error_here_doc(int fd, int errornum);
void	error_open(t_cmd arg, int **pipefd, int *fd, int errornum);
void	path_free(t_cmd arg);
void	execve_free(t_cmd arg, char **cmd, int i);

size_t	ft_strlen(const char *str);
int		ft_strlen_sep(const char *str, const char c);
int		ft_strcmp(const char *s1, const char *s2);
int		ft_strncmp(const char *s1, const char *s2, int n);
char	*get_next_line(int fd);
char	**ft_split(const char *str, char c);
int		count_words(const char *str, char c);

#endif