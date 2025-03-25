/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tssaito <tssaito@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 10:43:45 by tssaito           #+#    #+#             */
/*   Updated: 2025/03/24 11:04:54 by tssaito          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "includes.h"

size_t	ft_strlen(const char *s);
int		ft_strlen_sep(const char *str, const char c);
int		ft_strcmp(const char *s1, const char *s2);
int		ft_strncmp(const char *s1, const char *s2, int n);
size_t	ft_strncpy(char *dest, const char *src, size_t size);
char	*ft_strdup(const char *s);
char	**ft_split(const char *str, char c);
char	**ft_split_sep(const char *str, char **sep, int length);
int		count_words(const char *str, char c);
int		count_sep(const char *str, char **sep, int length, int count);
char	*ft_substr(const char *s, unsigned int start, size_t len);
void	ft_strcpy(char *new_str, char *str);
int		ft_atoi(const char *str);
size_t	ft_strlcat(char *dst, const char *src, size_t dsize);
size_t	ft_strlcpy(char *dst, const char *src, size_t dsize);
int		ft_isspace(char c);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strjoin_three(char const *s1, char const *s2, char const *s3);
char	*ft_strndup(const char *s, int n);
void	ft_putendl_fd(char *s, int fd);
void	ft_putstr_fd(char *s, int fd);
char	*ft_itoa(int n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
int		ft_isalnum(int c);
int		ft_isdigit(int c);

/* free_utils */
void	free_strs(char **strs);
void	free_tokens(t_tokens **tokens);
void	free_one_token(t_tokens **tokens, t_tokens *rm_token);
void	free_words(char **words, int size);
void	free_lst_status_(t_status *st_head, char *cmds);

#endif
